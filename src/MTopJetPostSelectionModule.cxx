#include <iostream>
#include <memory>

#include <UHH2/core/include/AnalysisModule.h>
#include <UHH2/core/include/Event.h>
#include <UHH2/core/include/Selection.h>

#include <UHH2/common/include/CleaningModules.h>
#include <UHH2/common/include/NSelections.h>
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

#include <UHH2/common/include/ElectronHists.h>
#include <UHH2/common/include/EventHists.h>
#include <UHH2/common/include/MuonHists.h>
#include <UHH2/common/include/JetHists.h>
#include <UHH2/common/include/TTbarGenHists.h>
#include <UHH2/MTopJet/include/MTopJetHists.h>


#include <UHH2/MTopJet/include/ModuleBASE.h>
#include <UHH2/MTopJet/include/MTopJetSelections.h>
#include <UHH2/MTopJet/include/MTopJetUtils.h>

class MTopJetPostSelectionModule : public ModuleBASE {

 public:
  explicit MTopJetPostSelectionModule(uhh2::Context&);
  virtual bool process(uhh2::Event&) override;

 protected:
  enum lepton { muon, elec };
  lepton channel_;

  // cleaners
  std::unique_ptr<MuonCleaner>     muoSR_cleaner;
  std::unique_ptr<ElectronCleaner> eleSR_cleaner;

  std::unique_ptr<JetCleaner>                      jet_IDcleaner;
  std::unique_ptr<JetCorrector>                    jet_corrector;
  std::unique_ptr<GenericJetResolutionSmearer>     jetER_smearer;
  std::unique_ptr<JetLeptonCleaner_by_KEYmatching> topjetlepton_cleaner;
  std::unique_ptr<JetCleaner>                      jet_cleaner1;
  std::unique_ptr<JetCleaner>                      jet_cleaner2;
  std::unique_ptr<JetLeptonCleaner>                jetlep_cleaner;

  std::unique_ptr<JetCleaner>                  topjet_IDcleaner;
  std::unique_ptr<TopJetCorrector>             topjet_corrector;
  std::unique_ptr<SubJetCorrector>             topjet_subjet_corrector;
  std::unique_ptr<GenericJetResolutionSmearer> topjetER_smearer;
  std::unique_ptr<TopJetLeptonDeltaRCleaner>   topjetleptondeltaR_cleaner;
  std::unique_ptr<TopJetCleaner>               topjet_cleaner;
 
  // selections
 
  std::unique_ptr<uhh2::Selection> topjet_sel;
  std::unique_ptr<uhh2::Selection> topjetB_sel;
  std::unique_ptr<uhh2::Selection> topjetC_sel;
  std::unique_ptr<uhh2::Selection> topjetD_sel;
  std::unique_ptr<uhh2::Selection> deltarcut_sel;
  std::unique_ptr<uhh2::Selection> topmass_sel;
  std::unique_ptr<uhh2::Selection> met_sel;

  std::unique_ptr<uhh2::AnalysisModule> ttgenprod;

  // store Hist collection as member variables
  std::unique_ptr<Hists> 
  h_cleaner_event, h_cleaner_elec, h_cleaner_muon, h_cleaner_jets, h_cleaner_topjets, h_cleaner_MTopJetHists,
    h_topjet_event, h_topjet_elec, h_topjet_muon, h_topjet_jets, h_topjet_topjets, h_topjet_MTopJetHists, 
    h_toplepdR_event, h_toplepdR_elec, h_toplepdR_muon, h_toplepdR_jets, h_toplepdR_topjets, h_toplepdR_MTopJetHists, 
    h_topmass_event, h_topmass_elec, h_topmass_muon, h_topmass_jets, h_topmass_topjets, h_topmass_MTopJetHists, 

    h_topjetB_event, h_topjetB_elec, h_topjetB_muon, h_topjetB_jets, h_topjetB_topjets, h_topjetB_MTopJetHists,
    h_toplepdRB_event, h_toplepdRB_elec, h_toplepdRB_muon, h_toplepdRB_jets, h_toplepdRB_topjets, h_toplepdRB_MTopJetHists,
    h_topmassB_event, h_topmassB_elec, h_topmassB_muon, h_topmassB_jets, h_topmassB_topjets, h_topmassB_MTopJetHists,

    h_topjetC_event, h_topjetC_elec, h_topjetC_muon, h_topjetC_jets, h_topjetC_topjets, h_topjetC_MTopJetHists,
    h_toplepdRC_event, h_toplepdRC_elec, h_toplepdRC_muon, h_toplepdRC_jets, h_toplepdRC_topjets, h_toplepdRC_MTopJetHists,
    h_topmassC_event, h_topmassC_elec, h_topmassC_muon, h_topmassC_jets, h_topmassC_topjets, h_topmassC_MTopJetHists,

    h_topjetD_event, h_topjetD_elec, h_topjetD_muon, h_topjetD_jets, h_topjetD_topjets, h_topjetD_MTopJetHists,
    h_toplepdRD_event, h_toplepdRD_elec, h_toplepdRD_muon, h_toplepdRD_jets, h_toplepdRD_topjets, h_toplepdRD_MTopJetHists,
    h_topmassD_event, h_topmassD_elec, h_topmassD_muon, h_topmassD_jets, h_topmassD_topjets, h_topmassD_MTopJetHists;    
};

MTopJetPostSelectionModule::MTopJetPostSelectionModule(uhh2::Context& ctx){

  //// CONFIGURATION
  const bool isMC = (ctx.get("dataset_type") == "MC");
  const std::string& keyword = ctx.get("keyword");

  bool deltaR = false;
  bool TopLep = false;
  bool noCleaner = false;

  if(keyword == "v01") TopLep = true;
  else if(keyword == "v02") deltaR = true;
  else if(keyword == "v03") noCleaner = true;

  //// COMMON MODULES


  ////

  //// OBJ CLEANING
  std::vector<std::string> JEC_AK4, JEC_AK8;
  if(isMC){
    JEC_AK4 = JERFiles::Fall15_25ns_L123_AK4PFchs_MC;
    JEC_AK8 = JERFiles::Fall15_25ns_L123_AK8PFchs_MC;
  }
  else {
    JEC_AK4 = JERFiles::Fall15_25ns_L123_AK4PFchs_DATA;
    JEC_AK8 = JERFiles::Fall15_25ns_L123_AK8PFchs_DATA;
  }

  if(deltaR){
    topjetleptondeltaR_cleaner.reset(new TopJetLeptonDeltaRCleaner(.8));
  }
  else if(TopLep){
    topjetlepton_cleaner.reset(new JetLeptonCleaner_by_KEYmatching(ctx, JEC_AK8, "topjets")); 
  }
  else if(noCleaner){
  }
  topjet_cleaner.reset(new TopJetCleaner(ctx, TopJetId(PtEtaCut(200., 2.4))));


  //// EVENT SELECTION


  topjet_sel.reset(new NTopJetSelection(2, 2, TopJetId(PtEtaCut(200, 2.4))));
  topjetB_sel.reset(new NTopJetSelection(1, 2, TopJetId(PtEtaCut(300, 2.4))));
  topjetC_sel.reset(new NTopJetSelection(1, 2, TopJetId(PtEtaCut(400, 2.4))));
  topjetD_sel.reset(new NTopJetSelection(1, 2, TopJetId(PtEtaCut(500, 2.4))));
 
  topmass_sel.reset(new TopJetMassCut());
  
  deltarcut_sel.reset(new deltaRCut(.8));

  ////

  //// set up Hists classes:
  h_cleaner_event.reset(new EventHists(ctx, "00_cleaner_Event"));
  h_cleaner_elec.reset(new ElectronHists(ctx, "00_cleaner_Elec"));
  h_cleaner_muon.reset(new MuonHists(ctx, "00_cleaner_Muon"));
  h_cleaner_jets.reset(new JetHists(ctx, "00_cleaner_Jets"));
  h_cleaner_topjets.reset(new TopJetHists(ctx, "00_cleaner_TopJets"));
  h_cleaner_MTopJetHists.reset(new MTopJetHists(ctx, "00_cleaner_MTopJetHists"));

  h_topjet_event.reset(new EventHists(ctx, "01_topjet_Event"));
  h_topjet_elec.reset(new ElectronHists(ctx, "01_topjet_Elec"));
  h_topjet_muon.reset(new MuonHists(ctx, "01_topjet_Muon"));
  h_topjet_jets.reset(new JetHists(ctx, "01_topjet_Jets"));
  h_topjet_topjets.reset(new TopJetHists(ctx, "01_topjet_TopJets"));
  h_topjet_MTopJetHists.reset(new MTopJetHists(ctx, "01_topjet_MTopJetHists"));

  h_topmass_event.reset(new EventHists(ctx, "02_topmass_Event"));
  h_topmass_elec.reset(new ElectronHists(ctx, "02_topmass_Elec"));
  h_topmass_muon.reset(new MuonHists(ctx, "02_topmass_Muon"));
  h_topmass_jets.reset(new JetHists(ctx, "02_topmass_Jets"));
  h_topmass_topjets.reset(new TopJetHists(ctx, "02_topmass_TopJets"));
  h_topmass_MTopJetHists.reset(new MTopJetHists(ctx, "02_topmass_MTopJetHists"));

  h_toplepdR_event.reset(new EventHists(ctx, "03_toplepdR_Event"));
  h_toplepdR_elec.reset(new ElectronHists(ctx, "03_toplepdR_Elec"));
  h_toplepdR_muon.reset(new MuonHists(ctx, "03_toplepdR_Muon"));
  h_toplepdR_jets.reset(new JetHists(ctx, "03_toplepdR_Jets"));
  h_toplepdR_topjets.reset(new TopJetHists(ctx, "03_toplepdR_TopJets"));
  h_toplepdR_MTopJetHists.reset(new MTopJetHists(ctx, "03_toplepdR_MTopJetHists"));

  // Selection B
  h_topjetB_event.reset(new EventHists(ctx, "01B_topjet_Event"));
  h_topjetB_elec.reset(new ElectronHists(ctx, "01B_topjet_Elec"));
  h_topjetB_muon.reset(new MuonHists(ctx, "01B_topjet_Muon"));
  h_topjetB_jets.reset(new JetHists(ctx, "01B_topjet_Jets"));
  h_topjetB_topjets.reset(new TopJetHists(ctx, "01B_topjet_TopJets"));
  h_topjetB_MTopJetHists.reset(new MTopJetHists(ctx, "01B_topjet_MTopJetHists"));

  h_topmassB_event.reset(new EventHists(ctx, "02B_topmass_Event"));
  h_topmassB_elec.reset(new ElectronHists(ctx, "02B_topmass_Elec"));
  h_topmassB_muon.reset(new MuonHists(ctx, "02B_topmass_Muon"));
  h_topmassB_jets.reset(new JetHists(ctx, "02B_topmass_Jets"));
  h_topmassB_topjets.reset(new TopJetHists(ctx, "02B_topmass_TopJets"));
  h_topmassB_MTopJetHists.reset(new MTopJetHists(ctx, "02B_topmass_MTopJetHists"));

  h_toplepdRB_event.reset(new EventHists(ctx, "03B_toplepdR_Event"));
  h_toplepdRB_elec.reset(new ElectronHists(ctx, "03B_toplepdR_Elec"));
  h_toplepdRB_muon.reset(new MuonHists(ctx, "03B_toplepdR_Muon"));
  h_toplepdRB_jets.reset(new JetHists(ctx, "03B_toplepdR_Jets"));
  h_toplepdRB_topjets.reset(new TopJetHists(ctx, "03B_toplepdR_TopJets"));
  h_toplepdRB_MTopJetHists.reset(new MTopJetHists(ctx, "03B_toplepdR_MTopJetHists"));
 
  // Selection C
  h_topjetC_event.reset(new EventHists(ctx, "01C_topjet_Event"));
  h_topjetC_elec.reset(new ElectronHists(ctx, "01C_topjet_Elec"));
  h_topjetC_muon.reset(new MuonHists(ctx, "01C_topjet_Muon"));
  h_topjetC_jets.reset(new JetHists(ctx, "01C_topjet_Jets"));
  h_topjetC_topjets.reset(new TopJetHists(ctx, "01C_topjet_TopJets"));
  h_topjetC_MTopJetHists.reset(new MTopJetHists(ctx, "01C_topjet_MTopJetHists"));

  h_topmassC_event.reset(new EventHists(ctx, "02C_topmass_Event"));
  h_topmassC_elec.reset(new ElectronHists(ctx, "02C_topmass_Elec"));
  h_topmassC_muon.reset(new MuonHists(ctx, "02C_topmass_Muon"));
  h_topmassC_jets.reset(new JetHists(ctx, "02C_topmass_Jets"));
  h_topmassC_topjets.reset(new TopJetHists(ctx, "02C_topmass_TopJets"));
  h_topmassC_MTopJetHists.reset(new MTopJetHists(ctx, "02C_topmass_MTopJetHists"));

  h_toplepdRC_event.reset(new EventHists(ctx, "03C_toplepdR_Event"));
  h_toplepdRC_elec.reset(new ElectronHists(ctx, "03C_toplepdR_Elec"));
  h_toplepdRC_muon.reset(new MuonHists(ctx, "03C_toplepdR_Muon"));
  h_toplepdRC_jets.reset(new JetHists(ctx, "03C_toplepdR_Jets"));
  h_toplepdRC_topjets.reset(new TopJetHists(ctx, "03C_toplepdR_TopJets"));
  h_toplepdRC_MTopJetHists.reset(new MTopJetHists(ctx, "03C_toplepdR_MTopJetHists"));
  
  // Selection D
  h_topjetD_event.reset(new EventHists(ctx, "01D_topjet_Event"));
  h_topjetD_elec.reset(new ElectronHists(ctx, "01D_topjet_Elec"));
  h_topjetD_muon.reset(new MuonHists(ctx, "01D_topjet_Muon"));
  h_topjetD_jets.reset(new JetHists(ctx, "01D_topjet_Jets"));
  h_topjetD_topjets.reset(new TopJetHists(ctx, "01D_topjet_TopJets"));
  h_topjetD_MTopJetHists.reset(new MTopJetHists(ctx, "01D_topjet_MTopJetHists"));

  h_topmassD_event.reset(new EventHists(ctx, "02D_topmass_Event"));
  h_topmassD_elec.reset(new ElectronHists(ctx, "02D_topmass_Elec"));
  h_topmassD_muon.reset(new MuonHists(ctx, "02D_topmass_Muon"));
  h_topmassD_jets.reset(new JetHists(ctx, "02D_topmass_Jets"));
  h_topmassD_topjets.reset(new TopJetHists(ctx, "02D_topmass_TopJets"));
  h_topmassD_MTopJetHists.reset(new MTopJetHists(ctx, "02D_topmass_MTopJetHists"));

  h_toplepdRD_event.reset(new EventHists(ctx, "03D_toplepdR_Event"));
  h_toplepdRD_elec.reset(new ElectronHists(ctx, "03D_toplepdR_Elec"));
  h_toplepdRD_muon.reset(new MuonHists(ctx, "03D_toplepdR_Muon"));
  h_toplepdRD_jets.reset(new JetHists(ctx, "03D_toplepdR_Jets"));
  h_toplepdRD_topjets.reset(new TopJetHists(ctx, "03D_toplepdR_TopJets"));
  h_toplepdRD_MTopJetHists.reset(new MTopJetHists(ctx, "03D_toplepdR_MTopJetHists"));
////

}

bool MTopJetPostSelectionModule::process(uhh2::Event& event){

  //// COMMON MODULES

  ////

  //// LEPTON selection

  ////

  //// JET selection

  ////

  //// Top Jet Selection
  topjetlepton_cleaner->process(event);
   //topjetleptondeltaR_cleaner->process(event); 
  topjet_cleaner->process(event);
  sort_by_pt<TopJet>(*event.topjets); // Sort TopJets by pT


  h_cleaner_event->fill(event);
  h_cleaner_elec->fill(event);
  h_cleaner_muon->fill(event);
  h_cleaner_jets->fill(event);
  h_cleaner_topjets->fill(event);
  h_cleaner_MTopJetHists->fill(event);

  /* 2nd AK8 jet selection */
  const bool pass_topjet = topjet_sel->passes(event);
  if(!pass_topjet) return false;

  h_topjet_event->fill(event);
  h_topjet_elec->fill(event);
  h_topjet_muon->fill(event);
  h_topjet_jets->fill(event);
  h_topjet_topjets->fill(event);
  h_topjet_MTopJetHists->fill(event);

  //--------- B Selection with 1 TopJet pT > 300 --------------------
  /* 1st AK8 jet selection */
  const bool pass_topjetB = topjetB_sel->passes(event);
  if(pass_topjetB){

    h_topjetB_event->fill(event);
    h_topjetB_elec->fill(event);
    h_topjetB_muon->fill(event);
    h_topjetB_jets->fill(event);
    h_topjetB_topjets->fill(event);
    h_topjetB_MTopJetHists->fill(event);

    /* Top Jet Mass Cut M1 > M2 */
    const bool pass_topmass = topmass_sel->passes(event);
    if(pass_topmass){
      h_topmassB_event->fill(event);
      h_topmassB_elec->fill(event);
      h_topmassB_muon->fill(event);
      h_topmassB_jets->fill(event);
      h_topmassB_topjets->fill(event);
      h_topmassB_MTopJetHists->fill(event);

      /* delta R (lep, topjet2) < 0.8  */
      const bool pass_deltaR = deltarcut_sel->passes(event);
      if(pass_deltaR){
	h_toplepdRB_event->fill(event);
	h_toplepdRB_elec->fill(event);
	h_toplepdRB_muon->fill(event);
	h_toplepdRB_jets->fill(event);
	h_toplepdRB_topjets->fill(event);
	h_toplepdRB_MTopJetHists->fill(event);
      }
    }
  }
  //--------------------------------------------------------------------

  //--------- C Selection with 1 TopJet pT > 400 --------------------
  /* 1st AK8 jet selection */
  const bool pass_topjetC = topjetC_sel->passes(event);
  if(pass_topjetC){

    h_topjetC_event->fill(event);
    h_topjetC_elec->fill(event);
    h_topjetC_muon->fill(event);
    h_topjetC_jets->fill(event);
    h_topjetC_topjets->fill(event);
    h_topjetC_MTopJetHists->fill(event);

    /* Top Jet Mass Cut M1 > M2 */
    const bool pass_topmass = topmass_sel->passes(event);
    if(pass_topmass){
      h_topmassC_event->fill(event);
      h_topmassC_elec->fill(event);
      h_topmassC_muon->fill(event);
      h_topmassC_jets->fill(event);
      h_topmassC_topjets->fill(event);
      h_topmassC_MTopJetHists->fill(event);

      /* delta R (lep, topjet2) < 0.8  */
      const bool pass_deltaR = deltarcut_sel->passes(event);
      if(pass_deltaR){
	h_toplepdRC_event->fill(event);
	h_toplepdRC_elec->fill(event);
	h_toplepdRC_muon->fill(event);
	h_toplepdRC_jets->fill(event);
	h_toplepdRC_topjets->fill(event);
	h_toplepdRC_MTopJetHists->fill(event);
      }
    }
  }
  //--------------------------------------------------------------------

  //--------- D Selection with 1 TopJet pT > 500 --------------------
  /* 1st AK8 jet selection */
  const bool pass_topjetD = topjetD_sel->passes(event);
  if(pass_topjetD){
    h_topjetD_event->fill(event);
    h_topjetD_elec->fill(event);
    h_topjetD_muon->fill(event);
    h_topjetD_jets->fill(event);
    h_topjetD_topjets->fill(event);
    h_topjetD_MTopJetHists->fill(event);

    /* Top Jet Mass Cut M1 > M2 */
    const bool pass_topmass = topmass_sel->passes(event);
    if(pass_topmass){
      h_topmassD_event->fill(event);
      h_topmassD_elec->fill(event);
      h_topmassD_muon->fill(event);
      h_topmassD_jets->fill(event);
      h_topmassD_topjets->fill(event);
      h_topmassD_MTopJetHists->fill(event);

      /* delta R (lep, topjet2) < 0.8  */
      const bool pass_deltaR = deltarcut_sel->passes(event);
      if(pass_deltaR){
	h_toplepdRD_event->fill(event);
	h_toplepdRD_elec->fill(event);
	h_toplepdRD_muon->fill(event);
	h_toplepdRD_jets->fill(event);
	h_toplepdRD_topjets->fill(event);
	h_toplepdRD_MTopJetHists->fill(event);
      }
    }
  }
  //--------------------------------------------------------------------

  //--------- Continue A Selection -------------------------------------

  /* Top Jet Mass Cut M1 > M2 */
  const bool pass_topmass = topmass_sel->passes(event);
  if(!pass_topmass) return false;

  h_topmass_event->fill(event);
  h_topmass_elec->fill(event);
  h_topmass_muon->fill(event);
  h_topmass_jets->fill(event);
  h_topmass_topjets->fill(event);
  h_topmass_MTopJetHists->fill(event);

  /* delta R (lep, topjet2) < 0.8  */
  const bool pass_deltaR = deltarcut_sel->passes(event);
  if(!pass_deltaR) return false;
  h_toplepdR_event->fill(event);
  h_toplepdR_elec->fill(event);
  h_toplepdR_muon->fill(event);
  h_toplepdR_jets->fill(event);
  h_toplepdR_topjets->fill(event);
  h_toplepdR_MTopJetHists->fill(event);


  ////
return true;
}

UHH2_REGISTER_ANALYSIS_MODULE(MTopJetPostSelectionModule)