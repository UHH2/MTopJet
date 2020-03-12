#include "UHH2/MTopJet/include/JetCorrections_xcone.h"
#include "UHH2/common/include/Utils.h"
#include "UHH2/core/include/Utils.h"

#include "UHH2/JetMETObjects/interface/FactorizedJetCorrector.h"
#include "UHH2/JetMETObjects/interface/JetCorrectorParameters.h"

#include <string>

using namespace std;
using namespace uhh2;

// maybe define new JERFiles for every uncertainty source?
// then UncertSource Files would have to be split
// How to construct JetCorrectionUncertainty Class with UncertaintySources:
// JetCorrectionUncertainty *total = new JetCorrectionUncertainty(*(new JetCorrectorParameters("Summer13_V4_DATA_UncertaintySources_AK5PFchs.txt", "Total")));



// JEC_factor_raw has to be set to a non 0 value for JEC
std::vector<TopJet> set_JEC_factor(std::vector<TopJet> jets){
  Jet new_subjet;
  vector<Jet> new_subjets;
  TopJet new_fatjet;
  vector<TopJet> new_fatjets;
  for(unsigned int i=0; i<jets.size(); i++){
    new_fatjet = jets.at(i);
    new_subjets.clear();
    for(unsigned int j=0; j<new_fatjet.subjets().size(); j++){
      new_subjet = jets.at(i).subjets().at(j);
      new_subjet.set_JEC_factor_raw(1.);
      new_subjets.push_back(new_subjet);
    }
    new_fatjet.set_subjets(new_subjets);
    new_fatjets.push_back(new_fatjet);
  }
  return new_fatjets;
}

JetCorrections_xcone::JetCorrections_xcone(){}

void JetCorrections_xcone::init(uhh2::Context & ctx, const std::string& jet_collection_rec){
  h_topjets = ctx.get_handle<std::vector<TopJet>>(jet_collection_rec);

  // setup JEC for XCone
  isMC = (ctx.get("dataset_type") == "MC");
  jet_corrector_MC.reset(new GenericSubJetCorrector(ctx, JERFiles::Summer16_23Sep2016_V4_L123_AK4PFchs_MC, jet_collection_rec));

  // vector<vector<string>> JERfiles_flavor = {JERFiles::Summer16_07Aug2017_V15_L123_AK4PFchs_MC_flavorUD,
  //                                           JERFiles::Summer16_07Aug2017_V15_L123_AK4PFchs_MC_flavorS,
  //                                           JERFiles::Summer16_07Aug2017_V15_L123_AK4PFchs_MC_flavorC,
  //                                           JERFiles::Summer16_07Aug2017_V15_L123_AK4PFchs_MC_flavorB,
  //                                           JERFiles::Summer16_07Aug2017_V15_L123_AK4PFchs_MC_flavorG};

  // jet_corrector_MC_flavor.reset(new GenericSubJetCorrector_flavor(ctx, JERfiles_flavor, jet_collection_rec));

  jet_corrector_BCD.reset(new GenericSubJetCorrector(ctx, JERFiles::Summer16_23Sep2016_V4_BCD_L123_AK4PFchs_DATA, jet_collection_rec));
  jet_corrector_EFearly.reset(new GenericSubJetCorrector(ctx, JERFiles::Summer16_23Sep2016_V4_EF_L123_AK4PFchs_DATA, jet_collection_rec));
  jet_corrector_FlateG.reset(new GenericSubJetCorrector(ctx, JERFiles::Summer16_23Sep2016_V4_G_L123_AK4PFchs_DATA, jet_collection_rec));
  jet_corrector_H.reset(new GenericSubJetCorrector(ctx, JERFiles::Summer16_23Sep2016_V4_H_L123_AK4PFchs_DATA, jet_collection_rec));
}

bool JetCorrections_xcone::process(uhh2::Event & event){
  std::vector<TopJet> jets = event.get(h_topjets);
  event.set(h_topjets, set_JEC_factor(jets)); // first set JEC_factor_raw to a non-0 value
  if(isMC){
    jet_corrector_MC->process(event);
    // jet_corrector_MC_flavor->process(event);
  }
  else{
    if(event.run <= runnr_BCD)         jet_corrector_BCD->process(event);
    else if(event.run < runnr_EFearly) jet_corrector_EFearly->process(event);
    else if(event.run <= runnr_FlateG) jet_corrector_FlateG->process(event);
    else if(event.run > runnr_FlateG)  jet_corrector_H->process(event);
    else throw runtime_error("Jet Correction: run number not covered by if-statements in process-routine.");
  }

  return true;
}


JER_Smearer_xcone::JER_Smearer_xcone(){}

void JER_Smearer_xcone::init(uhh2::Context & ctx, const std::string& jet_collection_rec, const std::string& jet_collection_gen, const std::string& fat_sub){
  h_rectopjets_ = ctx.get_handle<std::vector<TopJet> >   (jet_collection_rec);
  h_gentopjets_ = ctx.get_handle<std::vector<GenTopJet> >(jet_collection_gen);
  if(fat_sub == "sub") use_subjets = true;
  else                 use_subjets = false;
  JER_Smearer.reset(new GenericJetResolutionSmearer(ctx, jet_collection_rec, jet_collection_gen, false,  JERSmearing::SF_13TeV_2016_07Aug2017_v1, "Spring16_25nsV10_MC_PtResolution_AK4PFchs.txt"));
}

bool JER_Smearer_xcone::process(uhh2::Event & event){
  if(use_subjets){
    if(event.isRealData) return true;
    LorentzVector met;
    if(event.met) met = event.met->v4();
    std::vector<TopJet>*    rec_topjets;
    std::vector<GenTopJet>* gen_topjets;
    rec_topjets = &event.get(h_rectopjets_);
    gen_topjets = &event.get(h_gentopjets_);

    // match gentopjet to rectopjet
    double dR0 = deltaR(rec_topjets->at(0), gen_topjets->at(0));
    double dR1 = deltaR(rec_topjets->at(0), gen_topjets->at(1));
    unsigned int i0, i1;
    if(dR0 < dR1){
      i0 = 0;
      i1 = 1;
    }
    else {
      i0 = 1;
      i1 = 0;
    }

    // first smear subjets of topjet with index 0
    std::vector<Jet> rec_subjets0;
    for(unsigned int j=0; j<rec_topjets->at(0).subjets().size(); j++){
      rec_subjets0.push_back((rec_topjets->at(0).subjets().at(j)));
    }
    std::vector<Particle> gen_subjets0;
    for(unsigned int j=0; j<gen_topjets->at(i0).subjets().size(); j++){
      gen_subjets0.push_back((gen_topjets->at(i0).subjets().at(j)) );
    }
    JER_Smearer->apply_JER_smearing(rec_subjets0, gen_subjets0, met, 0.4, event.rho);
    rec_topjets->at(0).set_subjets(rec_subjets0);

    // then smear subjets of topjet with index 1
    std::vector<Jet> rec_subjets1;
    for(unsigned int j=0; j<rec_topjets->at(1).subjets().size(); j++){
      rec_subjets1.push_back((rec_topjets->at(1).subjets().at(j)));
    }
    std::vector<Particle> gen_subjets1;
    for(unsigned int j=0; j<gen_topjets->at(i1).subjets().size(); j++){
      gen_subjets1.push_back((gen_topjets->at(i1).subjets().at(j)) );
    }
    JER_Smearer->apply_JER_smearing(rec_subjets1, gen_subjets1, met, 0.4, event.rho);
    rec_topjets->at(1).set_subjets(rec_subjets1);
  }
  else

  JER_Smearer->process(event);

  return true;
}



// -----------------------------------------------------------------------------
/*
GenericSubJetCorrector::GenericSubJetCorrector(uhh2::Context & ctx, const std::vector<std::string> & filenames, const std::string & collectionname){
  corrector = build_corrector(filenames);
  direction = 0;
  // string file = "/nfs/dust/cms/user/schwarzd/CMSSW_8_0_24_patch1/src/UHH2/JECDatabase/textFiles/Summer16_23Sep2016V4_MC/Summer16_23Sep2016V4_MC_UncertaintySources_AK4PFchs.txt"
  // JetCorrectionUncertainty *total = new JetCorrectionUncertainty(*(new JetCorrectorParameters("Summer13_V4_DATA_UncertaintySources_AK5PFchs.txt", "Total")));
  jec_uncertainty = corrector_uncertainty(ctx, filenames, direction);
  h_jets = ctx.get_handle<std::vector<TopJet> >(collectionname);
}

bool GenericSubJetCorrector::process(uhh2::Event & event){
  const auto topjets = &event.get(h_jets);
  assert(topjets);
  for(auto & topjet : *topjets){
    auto subjets = topjet.subjets();
    for (auto & subjet : subjets) {
      correct_jet(*corrector, subjet, event, jec_uncertainty, direction);
    }
    topjet.set_subjets(move(subjets));
  }
  return true;
}
*/
