#include <iostream>
#include <memory>

#include <UHH2/core/include/AnalysisModule.h>
#include <UHH2/core/include/Event.h>
#include <UHH2/core/include/Selection.h>

#include <UHH2/common/include/CleaningModules.h>
#include <UHH2/common/include/NSelections.h>
#include <UHH2/common/include/LumiSelection.h>
#include <UHH2/common/include/TriggerSelection.h>
#include <UHH2/common/include/JetCorrections.h>
#include <UHH2/common/include/ObjectIdUtils.h>
#include <UHH2/common/include/MuonIds.h>
#include <UHH2/common/include/ElectronIds.h>
#include <UHH2/common/include/JetIds.h>
#include <UHH2/common/include/TopJetIds.h>
#include <UHH2/common/include/TTbarGen.h>
#include <UHH2/common/include/Utils.h>
#include <UHH2/common/include/AdditionalSelections.h>
#include <UHH2/common/include/TTbarGenHists.h>

#include <UHH2/MTopJet/include/ModuleBASE.h>
#include <UHH2/MTopJet/include/MTopJetSelections.h>
#include <UHH2/MTopJet/include/MTopJetGenSelections.h>
#include <UHH2/MTopJet/include/MTopJetGenHists.h>
#include <UHH2/MTopJet/include/MTopJetUtils.h>
#include <UHH2/MTopJet/include/JetCluster.h>

class MTopJetGenPostSelectionModule : public ModuleBASE {

 public:
  explicit MTopJetGenPostSelectionModule(uhh2::Context&);
  virtual bool process(uhh2::Event&) override;

 protected:

  // cleaners
  // std::unique_ptr<JetLeptonCleaner_by_KEYmatching> topjetlepton_cleaner;
  // selections
  std::unique_ptr<uhh2::AnalysisModule> ttgenprod;
  std::unique_ptr<uhh2::AnalysisModule> cleaner;

  // std::unique_ptr<uhh2::AnalysisModule> jetprod; // to produce Jets
  std::unique_ptr<uhh2::Selection> matching;
  std::unique_ptr<uhh2::Selection> topjetpt;
  std::unique_ptr<uhh2::Selection> masscut;
  std::unique_ptr<uhh2::Selection> n_genjets;
  std::unique_ptr<uhh2::Selection> deltaR;


  // store Hist collection as member variables
  std::unique_ptr<Hists> h_GenHists0a, h_GenHists0b, h_GenHists1, h_GenHists2, h_GenHists3, h_GenHists4, h_GenHists1_m, h_GenHists2_m, h_GenHists3_m, h_GenHists4_m, h_GenHists1_u, h_GenHists2_u, h_GenHists3_u, h_GenHists4_u;


};

MTopJetGenPostSelectionModule::MTopJetGenPostSelectionModule(uhh2::Context& ctx){

  //// CONFIGURATION
  // const bool isMC = (ctx.get("dataset_type") == "MC");
  
  // set up which jet to use
  const std::string jet_label("ak08_gen");
  float jet_radius = 0.8;

  ////

  //// COMMON MODULES

  const std::string ttbar_gen_label("ttbargen");
  ttgenprod.reset(new TTbarGenProducer(ctx, ttbar_gen_label, false));

  // // To produce Jets:
  // jetprod.reset(new JetProducer(ctx, jet_label));

  ////

  //// OBJ CLEANING
  cleaner.reset(new GenJetLeptonCleaner(ctx, jet_label, jet_radius));

  // set up Hists classes:
  h_GenHists0a.reset(new MTopJetGenHists(ctx, "GenHists before Cleaner", jet_label));
  h_GenHists0b.reset(new MTopJetGenHists(ctx, "GenHists after Cleaner", jet_label));

  h_GenHists1.reset(new MTopJetGenHists(ctx, "GenHists no Cut", jet_label));
  h_GenHists1_m.reset(new MTopJetGenHists(ctx, "GenHists no Cut matched", jet_label));
  h_GenHists1_u.reset(new MTopJetGenHists(ctx, "GenHists no Cut unmatched", jet_label));
  h_GenHists2.reset(new MTopJetGenHists(ctx, "GenHists pT Cut", jet_label));
  h_GenHists2_m.reset(new MTopJetGenHists(ctx, "GenHists pT Cut matched", jet_label));
  h_GenHists2_u.reset(new MTopJetGenHists(ctx, "GenHists pT Cut unmatched", jet_label));
  h_GenHists3.reset(new MTopJetGenHists(ctx, "GenHists masscut", jet_label));
  h_GenHists3_m.reset(new MTopJetGenHists(ctx, "GenHists masscut matched", jet_label));
  h_GenHists3_u.reset(new MTopJetGenHists(ctx, "GenHists masscut unmatched", jet_label));
  h_GenHists4.reset(new MTopJetGenHists(ctx, "GenHists deltaR", jet_label));
  h_GenHists4_m.reset(new MTopJetGenHists(ctx, "GenHists deltaR matched", jet_label));
  h_GenHists4_u.reset(new MTopJetGenHists(ctx, "GenHists deltaR unmatched", jet_label));

  // EVENT SELECTION
  n_genjets.reset(new NGenJets(ctx, jet_label, 2, 2));
  topjetpt.reset(new LeadingJetPT(ctx, jet_label, 400));
  deltaR.reset(new DeltaRCut(ctx, jet_label, jet_radius));
  masscut.reset(new MassCut(ctx, jet_label));
  matching.reset(new Matching(ctx, jet_label, jet_radius));
}

bool MTopJetGenPostSelectionModule::process(uhh2::Event& event){

  //  COMMON MODULES

  ttgenprod->process(event);
  // jetprod->process(event); // to produce jets

  h_GenHists0a->fill(event);
  cleaner->process(event);
  h_GenHists0b->fill(event);

  // ==2 Jets with pT > 150
  if(!(n_genjets->passes(event))) return false;
  h_GenHists1->fill(event);

  if(matching->passes(event)){
    h_GenHists1_m->fill(event);
  }
  if(!(matching->passes(event))){
    h_GenHists1_u->fill(event);
  }

  // pT cut on leading Jet
  if(!(topjetpt->passes(event))) return false;
  h_GenHists2->fill(event);

  if(matching->passes(event)){
    h_GenHists2_m->fill(event);
  }
  if(!(matching->passes(event))){
    h_GenHists2_u->fill(event);
  }

  // deltaR(lepton, 2nd jet) < jet radius
  if(!(deltaR->passes(event))) return false;
  h_GenHists3->fill(event);

  if(matching->passes(event)){
    h_GenHists3_m->fill(event);
  }
  if(!(matching->passes(event))){
    h_GenHists3_u->fill(event);
  }

  // m(1st jet) > m(2nd jet + lepton)
  if(!(masscut->passes(event))) return false;
  h_GenHists4->fill(event);

  if(matching->passes(event)){
    h_GenHists4_m->fill(event);
  }
  if(!(matching->passes(event))){
    h_GenHists4_u->fill(event);
  }
  return true; //false to delete all collections and keep hists
}

UHH2_REGISTER_ANALYSIS_MODULE(MTopJetGenPostSelectionModule)
