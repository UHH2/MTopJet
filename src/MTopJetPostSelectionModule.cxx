#include <iostream>
#include <memory>

#include <UHH2/core/include/AnalysisModule.h>
#include <UHH2/core/include/Event.h>
#include <UHH2/core/include/Selection.h>

#include <UHH2/common/include/Utils.h>
#include <UHH2/common/include/AdditionalSelections.h>

#include <UHH2/common/include/ElectronHists.h>
#include <UHH2/common/include/MuonHists.h>
#include <UHH2/common/include/JetHists.h>
#include <UHH2/common/include/TTbarGen.h>
#include <UHH2/common/include/TopPtReweight.h>
#include <UHH2/common/include/MCWeight.h>
#include <UHH2/common/include/MuonIds.h>
#include <UHH2/common/include/ElectronIds.h>

#include <UHH2/MTopJet/include/MTopJetHists.h>
#include <UHH2/MTopJet/include/CombineXCone.h>
#include <UHH2/MTopJet/include/ModuleBASE.h>
#include <UHH2/MTopJet/include/RecoSelections.h>
#include <UHH2/MTopJet/include/RecoSelections_topjet.h>
#include <UHH2/MTopJet/include/GenSelections.h>
#include <UHH2/MTopJet/include/RecoHists_xcone.h>
#include <UHH2/MTopJet/include/RecoHists_puppi.h>
#include <UHH2/MTopJet/include/RecoHists_topjet.h>
#include <UHH2/MTopJet/include/PDFHists.h>

#include <UHH2/MTopJet/include/GenHists_xcone.h>
#include <UHH2/MTopJet/include/GenHists_particles.h>
#include <UHH2/MTopJet/include/RecoGenHists_xcone.h>
#include <UHH2/MTopJet/include/MTopJetUtils.h>
#include <UHH2/MTopJet/include/AnalysisOutput.h>
#include <UHH2/MTopJet/include/SubjetHists_xcone.h>
#include "UHH2/MTopJet/include/RecoGenHists_subjets.h"
#include "UHH2/MTopJet/include/RecoGenHists_ak4.h"
#include "UHH2/MTopJet/include/CorrectionHists_subjets.h"
#include "UHH2/MTopJet/include/CorrectionFactor.h"
#include "UHH2/MTopJet/include/tt_width_reweight.h"
#include <UHH2/MTopJet/include/JetCorrections_xcone.h>
#include <UHH2/MTopJet/include/ElecTriggerSF.h>
#include "UHH2/MTopJet/include/WeightHists.h"

#include <vector>

/*
*******************************************************************
**************** TO DO ********************************************
*******************************************************************
- SF apllied to TTbar!
*******************************************************************
*******************************************************************
*/

class MTopJetPostSelectionModule : public ModuleBASE {

public:
  explicit MTopJetPostSelectionModule(uhh2::Context&);
  virtual bool process(uhh2::Event&) override;

protected:
  enum lepton { muon, elec };
  lepton channel_;

  // cleaners & Correctors
  // std::unique_ptr<uhh2::AnalysisModule> Correction;

  // selections
  std::unique_ptr<uhh2::Selection> njet_had;
  std::unique_ptr<uhh2::Selection> njet_lep;
  std::unique_ptr<uhh2::Selection> pt_sel;
  std::unique_ptr<uhh2::Selection> pt450_sel;
  std::unique_ptr<uhh2::Selection> pt530_sel;
  std::unique_ptr<uhh2::Selection> pt2_sel;
  std::unique_ptr<uhh2::Selection> eta_sel;
  std::unique_ptr<uhh2::Selection> pt350_gensel;
  std::unique_ptr<uhh2::Selection> pt350_sel;
  std::unique_ptr<uhh2::Selection> pt750_sel;
  std::unique_ptr<uhh2::Selection> mass_sel;
  std::unique_ptr<uhh2::Selection> pt_gensel;
  std::unique_ptr<uhh2::Selection> pt2_gensel;
  std::unique_ptr<uhh2::Selection> mass_gensel;
  std::unique_ptr<uhh2::Selection> pt_gensel23;
  std::unique_ptr<uhh2::Selection> mass_gensel23;
  std::unique_ptr<uhh2::Selection> matched_sub_GEN;
  std::unique_ptr<uhh2::Selection> matched_fat_GEN;
  std::unique_ptr<uhh2::Selection> matched_sub;
  std::unique_ptr<uhh2::Selection> matched_fat;
  std::unique_ptr<uhh2::Selection> subjet_quality;
  std::unique_ptr<uhh2::Selection> subjet_quality_gen;
  std::unique_ptr<uhh2::Selection> lepton_sel;
  std::unique_ptr<uhh2::Selection> muon_highpt_sel;
  std::unique_ptr<uhh2::Selection> lepton_sel_gen;
  std::unique_ptr<uhh2::Selection> lepton_Nsel_gen;

  std::unique_ptr<uhh2::Selection> njet_ak8;
  std::unique_ptr<uhh2::Selection> deltaR_ak8;
  std::unique_ptr<uhh2::Selection> pt_sel_ak8;
  std::unique_ptr<uhh2::Selection> mass_ak8;


  // get weight (with all SF and weight applied in previous cycle)
  Event::Handle<double>h_weight;

  // use top pt reweight module instead of derived sf?
  std::unique_ptr<TopPtReweight> ttbar_reweight;

  // handles for output
  Event::Handle<double>h_musf_central;
  uhh2::Event::Handle<TTbarGen>h_ttbargen;

  Event::Handle<bool>h_gensel_2;
  Event::Handle<bool>h_recsel_2;
  Event::Handle<bool>h_matched;
  Event::Handle<bool>h_measure_rec;
  Event::Handle<bool>h_gensel23;
  Event::Handle<bool>h_measure_gen;
  Event::Handle<bool>h_nomass_rec;
  Event::Handle<bool>h_nomass_gen;
  Event::Handle<bool>h_ptsub_rec;
  Event::Handle<bool>h_ptsub_gen;
  Event::Handle<bool>h_ptlepton_rec;
  Event::Handle<bool>h_ptlepton_gen;
  Event::Handle<bool>h_pt350_gen;
  Event::Handle<bool>h_pt350_rec;
  Event::Handle<bool>h_nobtag_rec;
  Event::Handle<bool>h_ttbar;
  Event::Handle<double>h_ttbar_SF;
  Event::Handle<double>h_mass_gen33;
  Event::Handle<double>h_mass_gen23;
  Event::Handle<double>h_mass_rec;
  Event::Handle<double>h_pt_gen33;
  Event::Handle<double>h_pt_gen23;
  Event::Handle<double>h_pt_rec;
  Event::Handle<double>h_genweight;
  Event::Handle<double>h_recweight;
  Event::Handle<double>h_genweight_ttfactor;
  Event::Handle<double>h_factor_2width;
  Event::Handle<double>h_factor_4width;
  Event::Handle<double>h_factor_8width;
  Event::Handle<std::vector<double>>h_bquark_pt;
  Event::Handle<std::vector<double>>h_pdf_weights;

  Event::Handle<std::vector<TopJet>>h_recjets_had;
  Event::Handle<std::vector<GenTopJet>>h_genjets23_had;
  Event::Handle<std::vector<GenTopJet>>h_genjets33_had;
  std::unique_ptr<uhh2::AnalysisModule> ttgenprod;

  //width reweight
  std::unique_ptr<tt_width_reweight> width2_reweight;
  std::unique_ptr<tt_width_reweight> width4_reweight;
  std::unique_ptr<tt_width_reweight> width8_reweight;

  //scale variation
  std::unique_ptr<AnalysisModule> scale_variation;
  std::unique_ptr<AnalysisModule> PUreweight;

  // store Hist collection as member variables
  std::unique_ptr<Hists> h_Muon_PreSel01, h_Elec_PreSel01, h_MTopJet_PreSel01, h_Jets_PreSel01, h_XCone_cor_PreSel01;
  std::unique_ptr<Hists> h_Muon_PreSel02, h_Elec_PreSel02, h_MTopJet_PreSel02, h_Jets_PreSel02, h_XCone_cor_PreSel02;
  std::unique_ptr<Hists> h_Muon_PreSel03, h_Elec_PreSel03, h_MTopJet_PreSel03, h_Jets_PreSel03, h_XCone_cor_PreSel03;
  std::unique_ptr<Hists> h_Muon_PreSel03b, h_Elec_PreSel03b, h_MTopJet_PreSel03b, h_Jets_PreSel03b, h_XCone_cor_PreSel03b;
  std::unique_ptr<Hists> h_Muon_PreSel04, h_Elec_PreSel04, h_MTopJet_PreSel04, h_Jets_PreSel04, h_XCone_cor_PreSel04;

  std::unique_ptr<Hists> h_Muon, h_Elec, h_MTopJet, h_Jets;
  std::unique_ptr<Hists> h_CorrectionHists, h_CorrectionHists_after, h_CorrectionHists_WJets;
  std::unique_ptr<Hists> h_RecGenHists_ak4, h_RecGenHists_ak4_noJEC;
  std::unique_ptr<Hists> h_weights01, h_weights02, h_weights03, h_weights04;

  std::unique_ptr<Hists> h_XCone_cor, h_XCone_jec, h_XCone_raw;
  std::unique_ptr<Hists> h_XCone_puppi;
  std::unique_ptr<Hists> h_XCone_cor_SF, h_XCone_jec_SF, h_XCone_raw_SF;
  std::unique_ptr<Hists> h_XCone_cor_SF_pt400, h_XCone_cor_SF_pt450, h_XCone_cor_SF_pt530;
  std::unique_ptr<Hists> h_XCone_cor_pt350, h_XCone_cor_noptcut;
  std::unique_ptr<Hists> h_XCone_cor_subjets, h_XCone_jec_subjets, h_XCone_raw_subjets;
  std::unique_ptr<Hists> h_XCone_cor_subjets_SF, h_XCone_jec_subjets_SF, h_XCone_raw_subjets_SF;
  std::unique_ptr<Hists> h_XCone_cor_Sel_noSel, h_XCone_cor_Sel_noMass, h_XCone_cor_Sel_nobtag, h_XCone_cor_Sel_pt350, h_XCone_cor_Sel_ptsub, h_XCone_cor_subjets_Sel_ptsub;
  std::unique_ptr<Hists> h_XCone_cor_SF_Sel_pt350, h_XCone_cor_SF_Sel_noMass;

  std::unique_ptr<Hists> h_XCone_cor_m, h_XCone_cor_u, h_XCone_cor_m_fat, h_XCone_cor_u_fat;

  std::unique_ptr<Hists> h_XCone_cor_PUlow, h_XCone_cor_PUmid,h_XCone_cor_PUhigh;
  std::unique_ptr<Hists> h_XCone_cor_PUlow_subjets, h_XCone_cor_PUmid_subjets, h_XCone_cor_PUhigh_subjets;

  std::unique_ptr<Hists> h_XCone_GEN_beforeMass, h_XCone_GEN_beforeMass_matched;

  std::unique_ptr<Hists> h_RecGenHists_lowPU, h_RecGenHists_medPU, h_RecGenHists_highPU, h_RecGenHists_lowPU_noJEC, h_RecGenHists_medPU_noJEC, h_RecGenHists_highPU_noJEC, h_RecGenHists_RecOnly_corr;
  std::unique_ptr<Hists> h_XCone_GEN_RecOnly, h_XCone_GEN_Both;
  std::unique_ptr<Hists> h_XCone_GEN_ST;
  std::unique_ptr<Hists> h_XCone_GEN_GenOnly, h_XCone_GEN_GenOnly_matched, h_XCone_GEN_GenOnly_unmatched, h_XCone_GEN_GenOnly_matched_fat, h_XCone_GEN_GenOnly_unmatched_fat;
  std::unique_ptr<Hists> h_RecGenHists_GenSelRecInfo, h_RecGenHists_GenSelRecInfo_lowPU, h_RecGenHists_GenSelRecInfo_midPU, h_RecGenHists_GenSelRecInfo_highPU;
  std::unique_ptr<Hists> h_RecGenHists_GenSelRecInfo_matched, h_RecGenHists_GenSelRecInfo_matched_lowPU, h_RecGenHists_GenSelRecInfo_matched_midPU, h_RecGenHists_GenSelRecInfo_matched_highPU;
  std::unique_ptr<Hists> h_RecGenHists_GenOnly;
  std::unique_ptr<Hists> h_RecGenHists_RecOnly;
  std::unique_ptr<Hists> h_RecGenHists_RecOnly_noJEC;
  std::unique_ptr<Hists> h_RecGenHists_Both, h_RecGenHists_Both_corr;
  std::unique_ptr<Hists> h_RecGenHists_subjets, h_RecGenHists_subjets_noJEC, h_RecGenHists_subjets_corrected, h_RecGenHists_subjets_matched;
  std::unique_ptr<Hists> h_RecGenHistsST_subjets, h_RecGenHistsST_subjets_noJEC, h_RecGenHistsST_subjets_corrected;
  std::unique_ptr<Hists> h_RecGenHists_subjets_lowPU, h_RecGenHists_subjets_medPU, h_RecGenHists_subjets_highPU;
  std::unique_ptr<Hists> h_RecGenHists_subjets_noJEC_lowPU, h_RecGenHists_subjets_noJEC_medPU, h_RecGenHists_subjets_noJEC_highPU;
  std::unique_ptr<Hists> h_RecGenHists_subjets_WJets, h_RecGenHists_subjets_noJEC_WJets;
  std::unique_ptr<Hists> h_GenParticles_RecOnly, h_GenParticles_GenOnly, h_GenParticles_Both;
  std::unique_ptr<Hists> h_GenParticles_SM, h_GenParticles_newWidth;

  std::unique_ptr<Hists> h_XCone_cor_migration_pt, h_XCone_cor_migration_pt350, h_XCone_cor_migration_mass, h_XCone_cor_migration_btag;
  std::unique_ptr<Hists> h_750_ak8, h_750_xcone;

  std::unique_ptr<Hists> h_XCone_GEN_Sel_measurement, h_XCone_GEN_Sel_noMass, h_XCone_GEN_Sel_pt350, h_XCone_GEN_Sel_ptsub;
  std::unique_ptr<Hists> h_PDFHists;

  bool isMC;    //define here to use it in "process" part
  bool isTTbar; //define here to use it in "process" part
  int counter;

  std::unique_ptr<uhh2::AnalysisModule> BTagScaleFactors, BTagReshape;
  std::unique_ptr<uhh2::AnalysisModule> muo_tight_noniso_SF, muo_trigger_SF;
  std::unique_ptr<uhh2::AnalysisModule> ele_id_SF, ele_trigger_SF, ele_reco_SF;


  string BTag_variation ="central";
  string MuScale_variation ="nominal";
  string MuTrigger_variation ="nominal";
  string ElID_variation ="nominal";
  string ElReco_variation ="nominal";
  string ElTrigger_variation ="nominal";
  string PU_variation ="nominal";

};

MTopJetPostSelectionModule::MTopJetPostSelectionModule(uhh2::Context& ctx){

  /*
  ██████ ████████ ██   ██
  ██         ██     ██ ██
  ██         ██      ███
  ██         ██     ██ ██
  ██████    ██    ██   ██
  */

  /*************************** CONFIGURATION **********************************************************************************/
  isMC = (ctx.get("dataset_type") == "MC");
  if(ctx.get("dataset_version") == "TTbar_Mtt0000to0700"  ||
  ctx.get("dataset_version") == "TTbar_Mtt0700to1000"  ||
  ctx.get("dataset_version") == "TTbar_Mtt1000toInft"  ||
  ctx.get("dataset_version") == "TTbar_Mtt0000to0700_jecup"  ||
  ctx.get("dataset_version") == "TTbar_Mtt0700to1000_jecup"  ||
  ctx.get("dataset_version") == "TTbar_Mtt1000toInft_jecup"  ||
  ctx.get("dataset_version") == "TTbar_Mtt0000to0700_jecdown"  ||
  ctx.get("dataset_version") == "TTbar_Mtt0700to1000_jecdown"  ||
  ctx.get("dataset_version") == "TTbar_Mtt1000toInft_jecdown"  ||
  ctx.get("dataset_version") == "TTbar_Mtt0000to0700_jerup"  ||
  ctx.get("dataset_version") == "TTbar_Mtt0700to1000_jerup"  ||
  ctx.get("dataset_version") == "TTbar_Mtt1000toInft_jerup"  ||
  ctx.get("dataset_version") == "TTbar_Mtt0000to0700_jerdown"  ||
  ctx.get("dataset_version") == "TTbar_Mtt0700to1000_jerdown"  ||
  ctx.get("dataset_version") == "TTbar_Mtt1000toInft_jerdown"  ||
  ctx.get("dataset_version") == "TTbar_Mtt0000to0700_corup"  ||
  ctx.get("dataset_version") == "TTbar_Mtt0700to1000_corup"  ||
  ctx.get("dataset_version") == "TTbar_Mtt1000toInft_corup"  ||
  ctx.get("dataset_version") == "TTbar_Mtt0000to0700_cordown"  ||
  ctx.get("dataset_version") == "TTbar_Mtt0700to1000_cordown"  ||
  ctx.get("dataset_version") == "TTbar_Mtt1000toInft_cordown"  ||
  ctx.get("dataset_version") == "TTbar_Mtt0000to0700_flavor_match"  ||
  ctx.get("dataset_version") == "TTbar_Mtt0700to1000_flavor_match"  ||
  ctx.get("dataset_version") == "TTbar_Mtt1000toInft_flavor_match"  ||
  ctx.get("dataset_version") == "TTbar_Mtt0000to0700_flavor_lightonly"  ||
  ctx.get("dataset_version") == "TTbar_Mtt0700to1000_flavor_lightonly"  ||
  ctx.get("dataset_version") == "TTbar_Mtt1000toInft_flavor_lightonly"  ||
  ctx.get("dataset_version") == "TTbar_Mtt0000to0700_flavor_leadingpt"  ||
  ctx.get("dataset_version") == "TTbar_Mtt0700to1000_flavor_leadingpt"  ||
  ctx.get("dataset_version") == "TTbar_Mtt1000toInft_flavor_leadingpt"  ||
  ctx.get("dataset_version") == "TTbar_Mtt0000to0700_flavor_btag"  ||
  ctx.get("dataset_version") == "TTbar_Mtt0700to1000_flavor_btag"  ||
  ctx.get("dataset_version") == "TTbar_Mtt1000toInft_flavor_btag"  ||
  ctx.get("dataset_version") == "TTbar_Mtt0000to0700_flavor_fractions"  ||
  ctx.get("dataset_version") == "TTbar_Mtt0700to1000_flavor_fractions"  ||
  ctx.get("dataset_version") == "TTbar_Mtt1000toInft_flavor_fractions"  ||
  ctx.get("dataset_version") == "TTbar_Mtt0000to0700_std_jec"  ||
  ctx.get("dataset_version") == "TTbar_Mtt0700to1000_std_jec"  ||
  ctx.get("dataset_version") == "TTbar_Mtt1000toInft_std_jec"  ||
  ctx.get("dataset_version") == "TTbar_mtop1665"       ||
  ctx.get("dataset_version") == "TTbar_mtop1695"       ||
  ctx.get("dataset_version") == "TTbar_mtop1715"       ||
  ctx.get("dataset_version") == "TTbar_mtop1735"       ||
  ctx.get("dataset_version") == "TTbar_mtop1755"       ||
  ctx.get("dataset_version") == "TTbar_mtop1785"       ||
  ctx.get("dataset_version") == "TTbar_fsrup"          ||
  ctx.get("dataset_version") == "TTbar_fsrdown"        ||
  ctx.get("dataset_version") == "TTbar_isrup"          ||
  ctx.get("dataset_version") == "TTbar_isrdown"        ||
  ctx.get("dataset_version") == "TTbar_hdampup"        ||
  ctx.get("dataset_version") == "TTbar_hdampdown"      ||
  ctx.get("dataset_version") == "TTbar_tuneup"         ||
  ctx.get("dataset_version") == "TTbar_tunedown"       ||
  ctx.get("dataset_version") == "TTbar_gluonmove"      ||
  ctx.get("dataset_version") == "TTbar_mpibased"       ||
  ctx.get("dataset_version") == "TTbar_amcatnlo-pythia"||
  ctx.get("dataset_version") == "TTbar_powheg-herwig") isTTbar = true;
  else  isTTbar = false;

  // ttbar gen
  const std::string ttbar_gen_label("ttbargen");
  if(isTTbar) ttgenprod.reset(new TTbarGenProducer(ctx, ttbar_gen_label, false));
  h_ttbargen=ctx.get_handle<TTbarGen>("ttbargen");


  const std::string& channel = ctx.get("channel", ""); //define Channel
  if     (channel == "muon") channel_ = muon;
  else if(channel == "elec") channel_ = elec;
  else {
    std::string log("MTopJetPostSelectionModule::MTopJetPostSelectionModule -- ");
    log += "invalid argument for 'channel' key in xml file (must be 'muon' or 'elec'): \""+channel+"\"";
    throw std::runtime_error(log);
  }

  counter = 0;

  // Top width reweigh
  width2_reweight.reset(new tt_width_reweight(ctx, 2.0));
  width4_reweight.reset(new tt_width_reweight(ctx, 4.0));
  width8_reweight.reset(new tt_width_reweight(ctx, 8.0));

  // Top PT reweight
  ttbar_reweight.reset(new TopPtReweight(ctx,0.0615,-0.0005,"","weight_ttbar",true)); // 13 TeV

  //scale variation
  scale_variation.reset(new MCScaleVariation(ctx));

  // PU reweighting
  PU_variation = ctx.get("PU_variation","central");
  PUreweight.reset(new MCPileupReweight(ctx, PU_variation));


  h_recjets_had = ctx.get_handle<std::vector<TopJet>>("XCone33_had_Combined_Corrected");
  if(isMC) h_genjets23_had = ctx.get_handle<std::vector<GenTopJet>>("GEN_XCone23_had_Combined");
  if(isMC) h_genjets33_had = ctx.get_handle<std::vector<GenTopJet>>("GEN_XCone33_had_Combined");

  /*************************** Setup Subjet Corrector **********************************************************************************/
  // Correction.reset(new CorrectionFactor(ctx, "xconeCHS_Corrected"));
  /*************************** Setup Selections **********************************************************************************/
  // RECO Selection
  // chose: XCone33_had_Combined_Corrected,  XCone33_had_Combined_noJEC,  XCone33_had_Combined
  const std::string& jet_label_had = "XCone33_had_Combined_Corrected";
  const std::string& jet_label_lep = "XCone33_lep_Combined_Corrected";

  njet_had.reset(new NJetXCone(ctx, jet_label_had, 1));
  njet_lep.reset(new NJetXCone(ctx, jet_label_lep, 1));

  subjet_quality.reset(new SubjetQuality(ctx, jet_label_had, 30, 2.5));
  pt_sel.reset(new LeadingRecoJetPT(ctx, jet_label_had, 400));
  pt450_sel.reset(new LeadingRecoJetPT(ctx, jet_label_had, 450));
  pt530_sel.reset(new LeadingRecoJetPT(ctx, jet_label_had, 530));
  pt2_sel.reset(new LeadingRecoJetPT(ctx, jet_label_lep, 10));
  eta_sel.reset(new LeadingRecoJetETA(ctx, jet_label_had, 2.5));
  pt350_sel.reset(new LeadingRecoJetPT(ctx, jet_label_had, 350));
  mass_sel.reset(new MassCutXCone(ctx, jet_label_had, jet_label_lep));

  MuonId muid = AndId<Muon>(MuonIDTight(), PtEtaCut(60., 2.4));
  ElectronId eleid = AndId<Electron>(PtEtaSCCut(60., 2.4), ElectronID_Spring16_tight);

  if(channel_ == muon) lepton_sel.reset(new NMuonSelection(1, -1, muid));
  else                 lepton_sel.reset(new NElectronSelection(1, -1, eleid));

  // GEN Selection
  if(channel_ == muon){
    lepton_sel_gen.reset(new GenMuonSel(ctx, 60.));
    lepton_Nsel_gen.reset(new GenMuonCount(ctx, 1, 1));
  }
  else{
    lepton_sel_gen.reset(new GenElecSel(ctx, 60.));
    lepton_Nsel_gen.reset(new GenElecCount(ctx, 1, 1));
  }
  subjet_quality_gen.reset(new SubjetQuality_gen(ctx, "GEN_XCone33_had_Combined", 30, 2.5));
  pt_gensel.reset(new LeadingJetPT_gen(ctx, "GEN_XCone33_had_Combined", 400));
  pt2_gensel.reset(new LeadingJetPT_gen(ctx, "GEN_XCone33_lep_Combined", 10));
  pt350_gensel.reset(new LeadingJetPT_gen(ctx, "GEN_XCone33_had_Combined", 350));
  mass_gensel.reset(new MassCut_gen(ctx, "GEN_XCone33_had_Combined", "GEN_XCone33_lep_Combined"));
  pt_gensel23.reset(new LeadingJetPT_gen(ctx, "GEN_XCone23_had_Combined", 400));
  mass_gensel23.reset(new MassCut_gen(ctx, "GEN_XCone23_had_Combined", "GEN_XCone23_lep_Combined"));
  matched_sub_GEN.reset(new Matching_XCone33GEN(ctx, "GEN_XCone33_had_Combined", true));
  matched_fat_GEN.reset(new Matching_XCone33GEN(ctx, "GEN_XCone33_had_Combined", false));



  // Selection for matching reco jets to gen particles
  if(isTTbar) matched_sub.reset(new Matching_XCone33(ctx, true));
  if(isTTbar) matched_fat.reset(new Matching_XCone33(ctx, false));

  // AK8 Selection / 750 GeV
  njet_ak8.reset(new NRecoJets_topjet(ctx, 200, 2, 2));
  deltaR_ak8.reset(new DeltaRCutReco_topjet(ctx, 1.2));
  pt_sel_ak8.reset(new LeadingRecoJetPT_topjet(ctx, 750));
  mass_ak8.reset(new MassCutReco_topjet(ctx));
  pt750_sel.reset(new LeadingRecoJetPT(ctx, jet_label_had, 750));

  // Scale factors
  BTag_variation = ctx.get("BTag_variation","central");
  MuScale_variation = ctx.get("MuScale_variation","nominal");
  MuTrigger_variation = ctx.get("MuTrigger_variation","nominal");
  ElID_variation = ctx.get("ElID_variation","nominal");
  ElReco_variation = ctx.get("ElReco_variation","nominal");
  ElTrigger_variation = ctx.get("ElTrigger_variation","nominal");

  BTagReshape.reset(new MCCSVv2ShapeSystematic(ctx, "jets", BTag_variation, "iterativefit", "", "BTagCalibration"));
  // BTagScaleFactors.reset(new MCBTagScaleFactor(ctx,CSVBTag::WP_TIGHT,"jets",BTag_variation));
  muo_tight_noniso_SF.reset(new MCMuonScaleFactor(ctx,"/nfs/dust/cms/user/schwarzd/CMSSW_8_0_24_patch1/src/UHH2/common/data/MuonID_EfficienciesAndSF_average_RunBtoH.root","MC_NUM_TightID_DEN_genTracks_PAR_pt_eta",1, "tightID", true, MuScale_variation));
  muo_trigger_SF.reset(new MCMuonScaleFactor(ctx,"/nfs/dust/cms/user/schwarzd/CMSSW_8_0_24_patch1/src/UHH2/common/data/MuonTrigger_EfficienciesAndSF_average_RunBtoH.root","IsoMu50_OR_IsoTkMu50_PtEtaBins",1, "muonTrigger", true, MuTrigger_variation));

  ele_trigger_SF.reset(new ElecTriggerSF(ctx, ElTrigger_variation, "eta_ptbins"));
  ele_reco_SF.reset(new MCElecScaleFactor(ctx, "/nfs/dust/cms/user/schwarzd/CMSSW_8_0_24_patch1/src/UHH2/common/data/egammaEffi.txt_EGM2D_RecEff_Moriond17.root", 1, "", ElReco_variation));
  ele_id_SF.reset(new MCElecScaleFactor(ctx, "/nfs/dust/cms/user/schwarzd/CMSSW_8_0_24_patch1/src/UHH2/common/data/egammaEffi.txt_EGM2D_CutBased_Tight_ID.root", 1, "", ElID_variation));
  /*************************** Set up Hists classes **********************************************************************************/

  //750GeV hists
  h_750_ak8.reset(new RecoHists_topjet(ctx, "750_ak8", "topjets"));
  h_750_xcone.reset(new RecoHists_xcone(ctx, "750_xcone", "cor"));

  // XCone Combined Jet
  h_XCone_raw.reset(new RecoHists_xcone(ctx, "XCone_raw", "raw"));
  h_XCone_cor.reset(new RecoHists_xcone(ctx, "XCone_cor", "cor"));
  h_XCone_jec.reset(new RecoHists_xcone(ctx, "XCone_jec", "jec"));
  h_XCone_raw_SF.reset(new RecoHists_xcone(ctx, "XCone_raw_SF", "raw"));
  h_XCone_cor_SF.reset(new RecoHists_xcone(ctx, "XCone_cor_SF", "cor"));
  h_XCone_jec_SF.reset(new RecoHists_xcone(ctx, "XCone_jec_SF", "jec"));

  h_XCone_cor_SF_pt400.reset(new RecoHists_xcone(ctx, "XCone_cor_SF_pt400", "cor"));
  h_XCone_cor_SF_pt450.reset(new RecoHists_xcone(ctx, "XCone_cor_SF_pt450", "cor"));
  h_XCone_cor_SF_pt530.reset(new RecoHists_xcone(ctx, "XCone_cor_SF_pt530", "cor"));

  h_XCone_cor_pt350.reset(new RecoHists_xcone(ctx, "XCone_cor_pt350", "cor"));
  h_XCone_cor_noptcut.reset(new RecoHists_xcone(ctx, "XCone_cor_noptcut", "cor"));

  h_XCone_puppi.reset(new RecoHists_puppi(ctx, "XCone_puppi"));

  // XCone Subjets
  h_XCone_jec_subjets.reset(new SubjetHists_xcone(ctx, "XCone_jec_subjets", "jec"));
  h_XCone_raw_subjets.reset(new SubjetHists_xcone(ctx, "XCone_raw_subjets", "raw"));
  h_XCone_cor_subjets.reset(new SubjetHists_xcone(ctx, "XCone_cor_subjets", "cor"));
  h_XCone_jec_subjets_SF.reset(new SubjetHists_xcone(ctx, "XCone_jec_subjets_SF", "jec"));
  h_XCone_raw_subjets_SF.reset(new SubjetHists_xcone(ctx, "XCone_raw_subjets_SF", "raw"));
  h_XCone_cor_subjets_SF.reset(new SubjetHists_xcone(ctx, "XCone_cor_subjets_SF", "cor"));

  // migrations from not passing rec cuts
  h_XCone_cor_migration_pt.reset(new RecoHists_xcone(ctx, "XCone_cor_migration_pt", "cor"));
  h_XCone_cor_migration_pt350.reset(new RecoHists_xcone(ctx, "XCone_cor_migration_pt350", "cor"));
  h_XCone_cor_migration_mass.reset(new RecoHists_xcone(ctx, "XCone_cor_migration_mass", "cor"));
  h_XCone_cor_migration_btag.reset(new RecoHists_xcone(ctx, "XCone_cor_migration_btag", "cor"));


  // Different REC Selection applied
  h_XCone_cor_Sel_noSel.reset(new RecoHists_xcone(ctx, "XCone_cor_Sel_noSel", "cor"));
  h_XCone_cor_Sel_noMass.reset(new RecoHists_xcone(ctx, "XCone_cor_Sel_noMass", "cor"));
  h_XCone_cor_Sel_pt350.reset(new RecoHists_xcone(ctx, "XCone_cor_Sel_pt350", "cor"));
  h_XCone_cor_Sel_nobtag.reset(new RecoHists_xcone(ctx, "XCone_cor_Sel_nobtag", "cor"));
  h_XCone_cor_Sel_ptsub.reset(new RecoHists_xcone(ctx, "XCone_cor_Sel_ptsub", "cor"));
  h_XCone_cor_subjets_Sel_ptsub.reset(new SubjetHists_xcone(ctx, "h_XCone_cor_subjets_Sel_ptsub", "cor"));
  h_XCone_cor_SF_Sel_noMass.reset(new RecoHists_xcone(ctx, "XCone_cor_SF_Sel_noMass", "cor"));
  h_XCone_cor_SF_Sel_pt350.reset(new RecoHists_xcone(ctx, "XCone_cor_SF_Sel_pt350", "cor"));

  // PU dependence
  h_XCone_cor_PUlow.reset(new RecoHists_xcone(ctx, "XCone_cor_PUlow", "cor"));
  h_XCone_cor_PUmid.reset(new RecoHists_xcone(ctx, "XCone_cor_PUmid", "cor"));
  h_XCone_cor_PUhigh.reset(new RecoHists_xcone(ctx, "XCone_cor_PUhigh", "cor"));

  h_XCone_cor_PUlow_subjets.reset(new SubjetHists_xcone(ctx, "XCone_cor_PUlow_subjets", "cor"));
  h_XCone_cor_PUmid_subjets.reset(new SubjetHists_xcone(ctx, "XCone_cor_PUmid_subjets", "cor"));
  h_XCone_cor_PUhigh_subjets.reset(new SubjetHists_xcone(ctx, "XCone_cor_PUhigh_subjets", "cor"));

  // Matching hists
  if(isTTbar) h_XCone_cor_m.reset(new RecoHists_xcone(ctx, "XCone_cor_matched", "cor"));
  if(isTTbar) h_XCone_cor_u.reset(new RecoHists_xcone(ctx, "XCone_cor_unmatched", "cor"));
  if(isTTbar) h_XCone_cor_m_fat.reset(new RecoHists_xcone(ctx, "XCone_cor_matched_fat", "cor"));
  if(isTTbar) h_XCone_cor_u_fat.reset(new RecoHists_xcone(ctx, "XCone_cor_unmatched_fat", "cor"));

  // Weight Hists
  h_weights01.reset(new WeightHists(ctx, "WeightHists01_noSF"));
  h_weights02.reset(new WeightHists(ctx, "WeightHists02_PU"));
  h_weights03.reset(new WeightHists(ctx, "WeightHists03_Lepton"));
  h_weights04.reset(new WeightHists(ctx, "WeightHists04_BTag"));


  h_MTopJet.reset(new MTopJetHists(ctx, "EventHists"));
  h_Muon.reset(new MuonHists(ctx, "MuonHists"));
  h_Elec.reset(new ElectronHists(ctx, "ElecHists"));
  h_Jets.reset(new JetHists(ctx, "JetHits"));

  h_MTopJet_PreSel01.reset(new MTopJetHists(ctx, "PreSel01_Event"));
  h_Muon_PreSel01.reset(new MuonHists(ctx, "PreSel01_Muon"));
  h_Elec_PreSel01.reset(new ElectronHists(ctx, "PreSel01_Elec"));
  h_Jets_PreSel01.reset(new JetHists(ctx, "PreSel01_Jet"));
  h_XCone_cor_PreSel01.reset(new RecoHists_xcone(ctx, "PreSel01_XCone", "cor"));

  h_MTopJet_PreSel02.reset(new MTopJetHists(ctx, "PreSel02_Event"));
  h_Muon_PreSel02.reset(new MuonHists(ctx, "PreSel02_Muon"));
  h_Elec_PreSel02.reset(new ElectronHists(ctx, "PreSel02_Elec"));
  h_Jets_PreSel02.reset(new JetHists(ctx, "PreSel02_Jet"));
  h_XCone_cor_PreSel02.reset(new RecoHists_xcone(ctx, "PreSel02_XCone", "cor"));

  h_MTopJet_PreSel03.reset(new MTopJetHists(ctx, "PreSel03_Event"));
  h_Muon_PreSel03.reset(new MuonHists(ctx, "PreSel03_Muon"));
  h_Elec_PreSel03.reset(new ElectronHists(ctx, "PreSel03_Elec"));
  h_Jets_PreSel03.reset(new JetHists(ctx, "PreSel03_Jet"));
  h_XCone_cor_PreSel03.reset(new RecoHists_xcone(ctx, "PreSel03_XCone", "cor"));

  h_MTopJet_PreSel03b.reset(new MTopJetHists(ctx, "PreSel03b_Event"));
  h_Muon_PreSel03b.reset(new MuonHists(ctx, "PreSel03b_Muon"));
  h_Elec_PreSel03b.reset(new ElectronHists(ctx, "PreSel03b_Elec"));
  h_Jets_PreSel03b.reset(new JetHists(ctx, "PreSel03b_Jet"));
  h_XCone_cor_PreSel03b.reset(new RecoHists_xcone(ctx, "PreSel03b_XCone", "cor"));

  h_MTopJet_PreSel04.reset(new MTopJetHists(ctx, "PreSel04_Event"));
  h_Muon_PreSel04.reset(new MuonHists(ctx, "PreSel04_Muon"));
  h_Elec_PreSel04.reset(new ElectronHists(ctx, "PreSel04_Elec"));
  h_Jets_PreSel04.reset(new JetHists(ctx, "PreSel04_Jet"));
  h_XCone_cor_PreSel04.reset(new RecoHists_xcone(ctx, "PreSel04_XCone", "cor"));

  if(isMC){
    if(isTTbar) h_CorrectionHists.reset(new CorrectionHists_subjets(ctx, "CorrectionHists", "jec"));
    if(isTTbar) h_CorrectionHists_after.reset(new CorrectionHists_subjets(ctx, "CorrectionHists_after", "cor"));
    h_CorrectionHists_WJets.reset(new CorrectionHists_subjets(ctx, "CorrectionHists_WJets", "jec"));


    h_XCone_GEN_Sel_measurement.reset(new GenHists_xcone(ctx, "XCone_GEN_Sel_measurement"));
    h_XCone_GEN_Sel_noMass.reset(new GenHists_xcone(ctx, "XCone_GEN_Sel_noMass"));
    h_XCone_GEN_Sel_pt350.reset(new GenHists_xcone(ctx, "XCone_GEN_Sel_pt350"));
    h_XCone_GEN_Sel_ptsub.reset(new GenHists_xcone(ctx, "XCone_GEN_Sel_ptsub"));

    h_XCone_GEN_GenOnly.reset(new GenHists_xcone(ctx, "XCone_GEN_GenOnly"));
    h_XCone_GEN_GenOnly_matched.reset(new GenHists_xcone(ctx, "XCone_GEN_GenOnly_matched"));
    h_XCone_GEN_GenOnly_unmatched.reset(new GenHists_xcone(ctx, "XCone_GEN_GenOnly_unmatched"));
    h_XCone_GEN_GenOnly_matched_fat.reset(new GenHists_xcone(ctx, "XCone_GEN_GenOnly_matched_fat"));
    h_XCone_GEN_GenOnly_unmatched_fat.reset(new GenHists_xcone(ctx, "XCone_GEN_GenOnly_unmatched_fat"));
    h_XCone_GEN_RecOnly.reset(new GenHists_xcone(ctx, "XCone_GEN_RecOnly"));
    h_XCone_GEN_Both.reset(new GenHists_xcone(ctx, "XCone_GEN_Both"));

    h_XCone_GEN_ST.reset(new GenHists_xcone(ctx, "XCone_GEN_ST"));

    h_XCone_GEN_beforeMass.reset(new GenHists_xcone(ctx, "XCone_GEN_beforeMass"));
    h_XCone_GEN_beforeMass_matched.reset(new GenHists_xcone(ctx, "XCone_GEN_beforeMass_matched"));

    h_GenParticles_GenOnly.reset(new GenHists_particles(ctx, "GenParticles_GenOnly"));
    h_GenParticles_RecOnly.reset(new GenHists_particles(ctx, "GenParticles_RecOnly"));
    h_GenParticles_Both.reset(new GenHists_particles(ctx, "GenParticles_Both"));

    h_GenParticles_SM.reset(new GenHists_particles(ctx, "GenParticles_SM"));
    h_GenParticles_newWidth.reset(new GenHists_particles(ctx, "GenParticles_newWidth"));

    h_RecGenHists_subjets.reset(new RecoGenHists_subjets(ctx, "RecGenHists_subjets", "jec"));
    h_RecGenHists_subjets_matched.reset(new RecoGenHists_subjets(ctx, "RecGenHists_subjets_matched", "jec"));
    h_RecGenHists_subjets_lowPU.reset(new RecoGenHists_subjets(ctx, "RecGenHists_subjets_lowPU", "jec"));
    h_RecGenHists_subjets_medPU.reset(new RecoGenHists_subjets(ctx, "RecGenHists_subjets_medPU", "jec"));
    h_RecGenHists_subjets_highPU.reset(new RecoGenHists_subjets(ctx, "RecGenHists_subjets_highPU", "jec"));
    h_RecGenHists_subjets_noJEC.reset(new RecoGenHists_subjets(ctx, "RecGenHists_subjets_noJEC", "raw"));
    h_RecGenHists_subjets_noJEC_lowPU.reset(new RecoGenHists_subjets(ctx, "RecGenHists_subjets_noJEC_lowPU", "raw"));
    h_RecGenHists_subjets_noJEC_medPU.reset(new RecoGenHists_subjets(ctx, "RecGenHists_subjets_noJEC_medPU", "raw"));
    h_RecGenHists_subjets_noJEC_highPU.reset(new RecoGenHists_subjets(ctx, "RecGenHists_subjets_noJEC_highPU", "raw"));
    h_RecGenHists_subjets_corrected.reset(new RecoGenHists_subjets(ctx, "RecGenHists_subjets_corrected", "cor"));

    h_RecGenHists_subjets_WJets.reset(new RecoGenHists_subjets(ctx, "RecGenHists_subjets_WJets", "jec"));
    h_RecGenHists_subjets_noJEC_WJets.reset(new RecoGenHists_subjets(ctx, "RecGenHists_subjets_noJEC_WJets", "raw"));

    h_RecGenHistsST_subjets.reset(new RecoGenHists_subjets(ctx, "RecGenHistsST_subjets", "jec"));
    h_RecGenHistsST_subjets_noJEC.reset(new RecoGenHists_subjets(ctx, "RecGenHistsST_subjets_noJEC", "raw"));
    h_RecGenHistsST_subjets_corrected.reset(new RecoGenHists_subjets(ctx, "RecGenHistsST_subjets_corrected", "cor"));


    h_RecGenHists_ak4.reset(new RecoGenHists_ak4(ctx, "RecGenHists_ak4", true));
    h_RecGenHists_ak4_noJEC.reset(new RecoGenHists_ak4(ctx, "RecGenHists_ak4_noJEC", false));

    h_RecGenHists_lowPU.reset(new RecoGenHists_xcone(ctx, "RecGenHists_lowPU", "jec"));
    h_RecGenHists_medPU.reset(new RecoGenHists_xcone(ctx, "RecGenHists_medPU", "jec"));
    h_RecGenHists_highPU.reset(new RecoGenHists_xcone(ctx, "RecGenHists_highPU", "jec"));
    h_RecGenHists_lowPU_noJEC.reset(new RecoGenHists_xcone(ctx, "RecGenHists_lowPU_noJEC", "raw"));
    h_RecGenHists_medPU_noJEC.reset(new RecoGenHists_xcone(ctx, "RecGenHists_medPU_noJEC", "raw"));
    h_RecGenHists_highPU_noJEC.reset(new RecoGenHists_xcone(ctx, "RecGenHists_highPU_noJEC", "raw"));

    // "true" for RecGenHists means to use jets with JEC applied
    h_RecGenHists_GenOnly.reset(new RecoGenHists_xcone(ctx, "RecGenHists_GenOnly", "jec"));
    h_RecGenHists_RecOnly.reset(new RecoGenHists_xcone(ctx, "RecGenHists_RecOnly", "jec"));
    h_RecGenHists_RecOnly_noJEC.reset(new RecoGenHists_xcone(ctx, "RecGenHists_RecOnly_noJEC", "raw"));
    h_RecGenHists_RecOnly_corr.reset(new RecoGenHists_xcone(ctx, "RecGenHists_RecOnly_corrected", "cor"));
    h_RecGenHists_Both.reset(new RecoGenHists_xcone(ctx, "RecGenHists_Both", "jec"));
    h_RecGenHists_Both_corr.reset(new RecoGenHists_xcone(ctx, "RecGenHists_Both_corrected", "cor"));
    h_RecGenHists_GenSelRecInfo.reset(new RecoGenHists_xcone(ctx, "RecGenHists_GenSelRecInfo", "cor"));
    h_RecGenHists_GenSelRecInfo_lowPU.reset(new RecoGenHists_xcone(ctx, "RecGenHists_GenSelRecInfo_lowPU", "cor"));
    h_RecGenHists_GenSelRecInfo_midPU.reset(new RecoGenHists_xcone(ctx, "RecGenHists_GenSelRecInfo_midPU", "cor"));
    h_RecGenHists_GenSelRecInfo_highPU.reset(new RecoGenHists_xcone(ctx, "RecGenHists_GenSelRecInfo_highPU", "cor"));
    h_RecGenHists_GenSelRecInfo_matched.reset(new RecoGenHists_xcone(ctx, "RecGenHists_GenSelRecInfo_matched", "cor"));
    h_RecGenHists_GenSelRecInfo_matched_lowPU.reset(new RecoGenHists_xcone(ctx, "RecGenHists_GenSelRecInfo_matched_lowPU", "cor"));
    h_RecGenHists_GenSelRecInfo_matched_midPU.reset(new RecoGenHists_xcone(ctx, "RecGenHists_GenSelRecInfo_matched_midPU", "cor"));
    h_RecGenHists_GenSelRecInfo_matched_highPU.reset(new RecoGenHists_xcone(ctx, "RecGenHists_GenSelRecInfo_matched_highPU", "cor"));
  }

  // PDF hists
  h_PDFHists.reset(new PDFHists(ctx, "PDFHists"));


  // get handles
  h_weight = ctx.get_handle<double>("weight");
  h_gensel_2 = ctx.get_handle<bool>("passed_gensel_2");
  h_recsel_2 = ctx.get_handle<bool>("passed_recsel_2");
  h_musf_central = ctx.get_handle<double>("passed_recsel_2");

  // undeclare event output (jet collections etc) to get small root files
  ctx.undeclare_all_event_output();

  // declare event output used for unfolding
  h_matched = ctx.declare_event_output<bool>("matched");
  h_measure_rec = ctx.declare_event_output<bool>("passed_measurement_rec");
  h_measure_gen = ctx.declare_event_output<bool>("passed_measurement_gen");
  h_nomass_rec = ctx.declare_event_output<bool>("passed_massmigration_rec");
  h_nomass_gen = ctx.declare_event_output<bool>("passed_massmigration_gen");
  h_ptsub_rec = ctx.declare_event_output<bool>("passed_subptmigration_rec");
  h_ptsub_gen = ctx.declare_event_output<bool>("passed_subptmigration_gen");
  h_pt350_rec = ctx.declare_event_output<bool>("passed_ptmigration_rec");
  h_pt350_gen = ctx.declare_event_output<bool>("passed_ptmigration_gen");
  h_nobtag_rec = ctx.declare_event_output<bool>("passed_btagmigration_rec");
  h_ptlepton_rec = ctx.declare_event_output<bool>("passed_leptonptmigration_rec");
  h_ptlepton_gen = ctx.declare_event_output<bool>("passed_leptonptmigration_gen");
  h_gensel23 = ctx.declare_event_output<bool>("passed_gensel23_full");
  h_ttbar = ctx.declare_event_output<bool>("is_TTbar");
  h_ttbar_SF = ctx.declare_event_output<double>("TTbar_SF");
  h_mass_gen23 = ctx.declare_event_output<double>("Mass_Gen23");
  h_mass_gen33 = ctx.declare_event_output<double>("Mass_Gen33");
  h_mass_rec = ctx.declare_event_output<double>("Mass_Rec");
  h_pt_gen23 = ctx.declare_event_output<double>("Pt_Gen23");
  h_pt_gen33 = ctx.declare_event_output<double>("Pt_Gen33");
  h_pt_rec = ctx.declare_event_output<double>("Pt_Rec");
  h_genweight = ctx.declare_event_output<double>("gen_weight");
  h_recweight = ctx.declare_event_output<double>("rec_weight");
  h_genweight_ttfactor = ctx.declare_event_output<double>("gen_weight_ttfactor");
  h_factor_2width = ctx.declare_event_output<double>("factor_2width");
  h_factor_4width = ctx.declare_event_output<double>("factor_4width");
  h_factor_8width = ctx.declare_event_output<double>("factor_8width");
  h_pdf_weights = ctx.declare_event_output<vector<double>>("pdf_weights");
  h_bquark_pt = ctx.declare_event_output<vector<double>>("bquark_pt");



  /*********************************************************************************************************************************/

}

bool MTopJetPostSelectionModule::process(uhh2::Event& event){

  /*
  ██████  ██████   ██████   ██████ ███████ ███████ ███████
  ██   ██ ██   ██ ██    ██ ██      ██      ██      ██
  ██████  ██████  ██    ██ ██      █████   ███████ ███████
  ██      ██   ██ ██    ██ ██      ██           ██      ██
  ██      ██   ██  ██████   ██████ ███████ ███████ ███████
  */


  // get bools for selections from root files
  bool passed_recsel;
  passed_recsel = event.get(h_recsel_2);
  bool passed_gensel33;
  passed_gensel33 = event.get(h_gensel_2);
  bool passed_gensel23;
  passed_gensel23 = event.get(h_gensel_2);
  ////


  // check if event has one had and one lep jet
  if( !(njet_had->passes(event)) ) return false;
  if( !(njet_lep->passes(event)) ) return false;
  ////

  // fill ttbargen class
  if(isTTbar) ttgenprod->process(event);
  ////


  /***************************  get jets to write mass *****************************************************************************************************/

  std::vector<TopJet> rec_hadjets = event.get(h_recjets_had);
  double mass_rec = rec_hadjets.at(0).v4().M();
  double pt_rec = rec_hadjets.at(0).v4().Pt();
  event.set(h_mass_rec, mass_rec);
  event.set(h_pt_rec, pt_rec);

  if(isMC){
    std::vector<GenTopJet> gen_hadjets23 = event.get(h_genjets23_had);
    std::vector<GenTopJet> gen_hadjets33 = event.get(h_genjets33_had);
    double mass_gen23 = gen_hadjets23.at(0).v4().M();
    double mass_gen33 = gen_hadjets33.at(0).v4().M();
    double pt_gen23 = gen_hadjets23.at(0).v4().Pt();
    double pt_gen33 = gen_hadjets33.at(0).v4().Pt();
    event.set(h_mass_gen23, mass_gen23);
    event.set(h_mass_gen33, mass_gen33);
    event.set(h_pt_gen23, pt_gen23);
    event.set(h_pt_gen33, pt_gen33);
  }
  else{
    event.set(h_mass_gen23, 0.); // set gen mass to 0 for data
    event.set(h_mass_gen33, 0.); // set gen mass to 0 for data
    event.set(h_pt_gen23, 0.);   // set gen pt to 0 for data
    event.set(h_pt_gen33, 0.);   // set gen pt to 0 for data
  }

  // also set bquark pt
  vector<double> bquark_pt = {0.0, 0.0};
  if(isTTbar){
    if(passed_gensel33 || passed_gensel23){
      const auto & ttbargen = event.get(h_ttbargen);
      GenParticle bot1 = ttbargen.bTop();
      GenParticle bot2 = ttbargen.bAntitop();
      bquark_pt[0] = bot1.pt();
      bquark_pt[1] = bot2.pt();
    }
  }
  event.set(h_bquark_pt, bquark_pt);



  /***************************  apply weight *****************************************************************************************************/
  // bool reweight_ttbar = false;       // apply ttbar reweight?
  bool scale_ttbar = true;           // match MC and data cross-section (for plots only)?
  double SF_tt = 0.846817;  // estimated in combined channel

  // get lumi weight = genweight (inkl scale variation)
  scale_variation->process(event); // here, it is only executed to be filled into the gen weight is has to be done again to appear in the event.weight
  double gen_weight = event.weight;

  // now get full weight from prev. Selection (weight = gen_weight * rec_weight)
  event.weight = event.get(h_weight);

  // FILL CONTROL PLOTS
  if(passed_recsel){
    h_MTopJet_PreSel01->fill(event);
    h_Muon_PreSel01->fill(event);
    h_Elec_PreSel01->fill(event);
    h_Jets_PreSel01->fill(event);
    h_XCone_cor_PreSel01->fill(event);
  }


  if(isTTbar)h_GenParticles_SM->fill(event);

  // choose if tt bar sample width should be reweighted
  double factor_2w, factor_4w, factor_8w;
  if(isTTbar){
    factor_2w = width2_reweight->get_factor(event);
    factor_4w = width4_reweight->get_factor(event);
    factor_8w = width8_reweight->get_factor(event);
  }
  else{
    factor_2w = 1.0;
    factor_4w = 1.0;
    factor_8w = 1.0;
  }

  scale_variation->process(event); // do this again because scale variation should change gen weight and event.weight, but not rec weight!!!

  // if(reweight_ttbar) ttbar_reweight->process(event);
  h_weights01->fill(event);

  /** store weights for pdf variation *********************/
  std::vector<double> pdf_weights;
  if(isTTbar){
    if(event.genInfo->systweights().size()){
      for(int i=0; i<100; i++){
        double pdf_weight = event.genInfo->systweights().at(i+9);
        pdf_weights.push_back(pdf_weight);
      }
    }
  }

  /** PU Reweighting *********************/
  PUreweight->process(event);
  h_weights02->fill(event);
  // FILL CONTROL PLOTS
  if(passed_recsel){
    h_MTopJet_PreSel02->fill(event);
    h_Muon_PreSel02->fill(event);
    h_Elec_PreSel02->fill(event);
    h_Jets_PreSel02->fill(event);
    h_XCone_cor_PreSel02->fill(event);
  }
  /** muon SF *********************/
  if(channel_ == muon){
    muo_tight_noniso_SF->process(event);
    muo_trigger_SF->process(event);
  }
  else{
    ele_id_SF->process(event);
    ele_reco_SF->process(event);
    ele_trigger_SF->process(event);
  }
  h_weights03->fill(event);
  // FILL CONTROL PLOTS
  if(passed_recsel){
    h_MTopJet_PreSel03->fill(event);
    h_Muon_PreSel03->fill(event);
    h_Elec_PreSel03->fill(event);
    h_Jets_PreSel03->fill(event);
    h_XCone_cor_PreSel03->fill(event);
  }
  /** b-tagging *********************/
  // first do cvs reshape (instead of SF)
  BTagReshape->process(event);
  if(passed_recsel){
    h_MTopJet_PreSel03b->fill(event);
    h_Muon_PreSel03b->fill(event);
    h_Elec_PreSel03b->fill(event);
    h_Jets_PreSel03b->fill(event);
    h_XCone_cor_PreSel03b->fill(event);
  }
  int jetbtagN(0);
  bool passed_btag;
  int jetbtagN_medium(0);
  bool passed_btag_medium;
  int jetbtagN_loose(0);
  bool passed_btag_loose;
  for(const auto& j : *event.jets){
    if(CSVBTag(CSVBTag::WP_TIGHT)(j, event)) ++jetbtagN;
    if(CSVBTag(CSVBTag::WP_MEDIUM)(j, event)) ++jetbtagN_medium;
    if(CSVBTag(CSVBTag::WP_LOOSE)(j, event)) ++jetbtagN_loose;
  }
  if(jetbtagN >= 1) passed_btag = true;
  else passed_btag = false;
  if(jetbtagN_medium >= 1) passed_btag_medium = true;
  else passed_btag_medium = false;
  if(jetbtagN_loose >= 1) passed_btag_loose = true;
  else passed_btag_loose = false;
  // BTagScaleFactors->process(event);
  h_weights04->fill(event);



  /** calculate recweight now *********************/
  double rec_weight;
  if(gen_weight==0)rec_weight = 0;
  else rec_weight = (event.weight)/gen_weight;


  /**********************************/
  bool passed_presel_rec = (passed_recsel && passed_btag);
  // FILL CONTROL PLOTS
  if(passed_presel_rec){
    h_MTopJet_PreSel04->fill(event);
    h_Muon_PreSel04->fill(event);
    h_Elec_PreSel04->fill(event);
    h_Jets_PreSel04->fill(event);
    h_XCone_cor_PreSel04->fill(event);
  }
  /*************************** Events have to pass topjet pt > 400 & Mass_jet1 > Mass_jet2 *******************************************************/

  bool pass_measurement_rec;
  bool pass_pt350migration_rec;
  bool pass_ptmigration_rec;
  bool pass_massmigration_rec;
  bool pass_btagmigration_rec;
  bool pass_WJets_sel;
  bool pass_subptmigration_rec;
  bool pass_leptonptmigration_rec;


  if(passed_recsel && pt_sel->passes(event) && pt2_sel->passes(event) && eta_sel->passes(event) && mass_sel->passes(event) && passed_btag && subjet_quality->passes(event) && lepton_sel->passes(event)) pass_measurement_rec = true;
  else pass_measurement_rec = false;

  if(passed_recsel && !pt_sel->passes(event) && pt2_sel->passes(event) && pt350_sel->passes(event) && eta_sel->passes(event) && mass_sel->passes(event) && passed_btag && subjet_quality->passes(event) && lepton_sel->passes(event) ) pass_pt350migration_rec = true;
  else pass_pt350migration_rec = false;

  if(passed_recsel && !pt_sel->passes(event) && pt2_sel->passes(event) && eta_sel->passes(event) && mass_sel->passes(event) && passed_btag && subjet_quality->passes(event) && lepton_sel->passes(event)) pass_ptmigration_rec = true;
  else pass_ptmigration_rec = false;

  if(passed_recsel && pt_sel->passes(event) && pt2_sel->passes(event) && eta_sel->passes(event) && !mass_sel->passes(event) && passed_btag && subjet_quality->passes(event) && lepton_sel->passes(event)) pass_massmigration_rec = true;
  else pass_massmigration_rec = false;

  if(passed_recsel && pt_sel->passes(event) && pt2_sel->passes(event) && eta_sel->passes(event) && mass_sel->passes(event) && !passed_btag && passed_btag_medium && subjet_quality->passes(event) && lepton_sel->passes(event)) pass_btagmigration_rec = true;
  else pass_btagmigration_rec = false;

  if(passed_recsel && pt_sel->passes(event) && pt2_sel->passes(event)  && eta_sel->passes(event) && !passed_btag_loose && subjet_quality->passes(event) && lepton_sel->passes(event)) pass_WJets_sel = true;
  else pass_WJets_sel = false;

  if(passed_recsel && pt_sel->passes(event) && pt2_sel->passes(event) && eta_sel->passes(event) && mass_sel->passes(event) && passed_btag && !subjet_quality->passes(event) && lepton_sel->passes(event)) pass_subptmigration_rec = true;
  else pass_subptmigration_rec = false;

  if(passed_recsel && pt_sel->passes(event) && pt2_sel->passes(event) && eta_sel->passes(event) && mass_sel->passes(event) && passed_btag && subjet_quality->passes(event) && !lepton_sel->passes(event)) pass_leptonptmigration_rec = true;
  else pass_leptonptmigration_rec = false;
  /*************************** Selection again on generator level (data events will not pass gen sel but will be stored if they pass rec sel)  ***/
  bool pass_measurement_gen;
  bool pass_pt350migration_gen;
  bool pass_massmigration_gen;
  bool pass_subptmigration_gen;
  bool pass_leptonptmigration_gen;

  if(isTTbar){
    if(!lepton_Nsel_gen->passes(event)) passed_gensel33 = false;

    if(passed_gensel33 && pt_gensel->passes(event) && pt2_gensel->passes(event) && mass_gensel->passes(event) && subjet_quality_gen->passes(event) && lepton_sel_gen->passes(event)) pass_measurement_gen = true;
    else pass_measurement_gen = false;

    if(passed_gensel33 && !pt_gensel->passes(event) && pt2_gensel->passes(event) && pt350_gensel->passes(event) && mass_gensel->passes(event) && subjet_quality_gen->passes(event) && lepton_sel_gen->passes(event)) pass_pt350migration_gen = true;
    else pass_pt350migration_gen = false;

    if(passed_gensel33 && pt_gensel->passes(event) && pt2_gensel->passes(event) && !mass_gensel->passes(event) && subjet_quality_gen->passes(event) && lepton_sel_gen->passes(event)) pass_massmigration_gen = true;
    else pass_massmigration_gen = false;

    if(passed_gensel33 && pt_gensel->passes(event) && pt2_gensel->passes(event) && mass_gensel->passes(event) && !subjet_quality_gen->passes(event) && lepton_sel_gen->passes(event)) pass_subptmigration_gen = true;
    else pass_subptmigration_gen = false;

    if(passed_gensel33 && pt_gensel->passes(event) && pt2_gensel->passes(event) && mass_gensel->passes(event) && subjet_quality_gen->passes(event) && !lepton_sel_gen->passes(event)) pass_leptonptmigration_gen = true;
    else pass_leptonptmigration_gen = false;
  }
  else if(!isTTbar){
    pass_measurement_gen = false;
    pass_pt350migration_gen = false;
    pass_massmigration_gen = false;
    pass_subptmigration_gen = false;
    pass_leptonptmigration_gen = false;
  }

  /***************************  750GeV SELECTION ***********************************************************************************/
  bool passes_750_ak8 = false;
  bool passes_750_xcone = false;

  if(passed_presel_rec && njet_ak8->passes(event)){
    if(deltaR_ak8->passes(event) && pt_sel_ak8->passes(event) && mass_ak8->passes(event)) passes_750_ak8 = true;
  }
  if(pass_measurement_rec && pt750_sel->passes(event)) passes_750_xcone = true;
  /******************************************************************************************************************************/


  /*************************** Pile Up bools  ***************************************************************************************************/
  bool lowPU = (event.pvs->size() <= 10);
  bool midPU = (event.pvs->size() > 10 && event.pvs->size() <= 20);
  bool highPU = (event.pvs->size() > 20);

  /*************************** fill hists with reco sel applied ***********************************************************************************/


  bool is_matched_sub = false;
  bool is_matched_fat = false;

  // Hists for 750GeV phase space
  if(passes_750_ak8) h_750_ak8->fill(event);
  if(passes_750_xcone) h_750_xcone->fill(event);



  // hists to see events that are generated in measurement phase-space, but reconstructed outside
  if(pass_measurement_gen){
    if(pass_pt350migration_rec) h_XCone_cor_migration_pt350->fill(event);
    if(pass_ptmigration_rec)    h_XCone_cor_migration_pt->fill(event);
    if(pass_massmigration_rec)  h_XCone_cor_migration_mass->fill(event);
    if(pass_btagmigration_rec)  h_XCone_cor_migration_btag->fill(event);
  }

  // see all events reconstructed outside measurement phase-space
  if(pass_ptmigration_rec) h_XCone_cor_noptcut->fill(event);
  if(pass_pt350migration_rec) h_XCone_cor_pt350->fill(event);
  if(pass_btagmigration_rec)  h_XCone_cor_Sel_nobtag->fill(event);
  if(pass_pt350migration_rec) h_XCone_cor_Sel_pt350->fill(event);
  if(pass_massmigration_rec)  h_XCone_cor_Sel_noMass->fill(event);
  if(pass_subptmigration_rec){
    h_XCone_cor_Sel_ptsub->fill(event);
    h_XCone_cor_subjets_Sel_ptsub->fill(event);
  }

  if(pass_measurement_gen)    h_XCone_GEN_Sel_measurement->fill(event);
  if(pass_pt350migration_gen) h_XCone_GEN_Sel_pt350->fill(event);
  if(pass_massmigration_gen)  h_XCone_GEN_Sel_noMass->fill(event);
  if(pass_subptmigration_gen) h_XCone_GEN_Sel_ptsub->fill(event);

  if(pass_measurement_gen || pass_massmigration_gen){
    h_XCone_GEN_beforeMass->fill(event);
    if(matched_sub_GEN->passes(event)) h_XCone_GEN_beforeMass_matched->fill(event);
  }

  if(pass_WJets_sel && isMC){
    h_CorrectionHists_WJets->fill(event);
    h_RecGenHists_subjets_WJets->fill(event);
    h_RecGenHists_subjets_noJEC_WJets->fill(event);
  }

  // fill resolution hists here without the subjet pt cut
  if(pass_measurement_gen || pass_subptmigration_gen){
    if(lowPU){
      if(isTTbar)h_RecGenHists_lowPU->fill(event);
      if(isTTbar)h_RecGenHists_lowPU_noJEC->fill(event);
      if(isTTbar)h_RecGenHists_subjets_lowPU->fill(event);
      if(isTTbar)h_RecGenHists_subjets_noJEC_lowPU->fill(event);
    }
    if(midPU){
      if(isTTbar)h_RecGenHists_medPU->fill(event);
      if(isTTbar)h_RecGenHists_medPU_noJEC->fill(event);
      if(isTTbar)h_RecGenHists_subjets_medPU->fill(event);
      if(isTTbar)h_RecGenHists_subjets_noJEC_medPU->fill(event);
    }
    if(highPU){
      if(isTTbar)h_RecGenHists_highPU->fill(event);
      if(isTTbar)h_RecGenHists_highPU_noJEC->fill(event);
      if(isTTbar)h_RecGenHists_subjets_highPU->fill(event);
      if(isTTbar)h_RecGenHists_subjets_noJEC_highPU->fill(event);
    }
    h_RecGenHists_RecOnly->fill(event);
    h_RecGenHists_RecOnly_noJEC->fill(event);
    h_RecGenHists_RecOnly_corr->fill(event);
    if(matched_fat->passes(event)) h_RecGenHists_subjets_matched->fill(event);
    h_RecGenHists_subjets->fill(event);
    h_RecGenHists_subjets_noJEC->fill(event);
    h_RecGenHists_subjets_corrected->fill(event);
    if(isTTbar) h_CorrectionHists->fill(event);
    if(isTTbar) h_CorrectionHists_after->fill(event);
    h_RecGenHists_ak4->fill(event);
    h_RecGenHists_ak4_noJEC->fill(event);
  }

  if(isMC){
    std::vector<GenTopJet> gen_hadjets33 = event.get(h_genjets33_had);
    double pt = gen_hadjets33.at(0).v4().Pt();
    double mjet = gen_hadjets33.at(0).v4().M();
    if(pt > 400 && mjet > 120){
      h_XCone_GEN_ST->fill(event);
      h_RecGenHistsST_subjets->fill(event);
      h_RecGenHistsST_subjets_noJEC->fill(event);
      h_RecGenHistsST_subjets_corrected->fill(event);
    }
  }

  if(pass_measurement_rec){
    if(isTTbar) h_PDFHists->fill(event);

    h_XCone_raw->fill(event);
    h_XCone_jec->fill(event);
    h_XCone_cor->fill(event);
    h_XCone_puppi->fill(event);

    h_XCone_raw_subjets->fill(event);
    h_XCone_jec_subjets->fill(event);
    h_XCone_cor_subjets->fill(event);

    if(isTTbar && scale_ttbar) event.weight *= SF_tt;

    h_XCone_raw_SF->fill(event);
    h_XCone_jec_SF->fill(event);
    h_XCone_cor_SF->fill(event);

    if(!pt450_sel->passes(event)) h_XCone_cor_SF_pt400->fill(event);
    if(pt450_sel->passes(event) && !pt530_sel->passes(event)) h_XCone_cor_SF_pt450->fill(event);
    if(pt530_sel->passes(event)) h_XCone_cor_SF_pt530->fill(event);

    h_XCone_raw_subjets_SF->fill(event);
    h_XCone_jec_subjets_SF->fill(event);
    h_XCone_cor_subjets_SF->fill(event);

    h_MTopJet->fill(event);
    h_Muon->fill(event);
    h_Elec->fill(event);
    h_Jets->fill(event);

    if(lowPU){
      h_XCone_cor_PUlow_subjets->fill(event);
      h_XCone_cor_PUlow->fill(event);
    }
    if(midPU){
      h_XCone_cor_PUmid_subjets->fill(event);
      h_XCone_cor_PUmid->fill(event);
    }
    if(highPU){
      h_XCone_cor_PUhigh_subjets->fill(event);
      h_XCone_cor_PUhigh->fill(event);
    }

    if(isTTbar){
      is_matched_sub = matched_sub->passes(event);
      is_matched_fat = matched_fat->passes(event);
      if(is_matched_sub) h_XCone_cor_m->fill(event);
      else h_XCone_cor_u->fill(event);
      if(is_matched_fat) h_XCone_cor_m_fat->fill(event);
      else h_XCone_cor_u_fat->fill(event);
    }

    if(isTTbar){
      h_XCone_GEN_RecOnly->fill(event);
      if(isTTbar) h_GenParticles_RecOnly->fill(event);

    }
  }



  /*************************** fill hists with gen sel applied *************************************************************************************/
  if(pass_measurement_gen){
    h_XCone_GEN_GenOnly->fill(event);
    if(matched_sub_GEN->passes(event)) h_XCone_GEN_GenOnly_matched->fill(event);
    else                               h_XCone_GEN_GenOnly_unmatched->fill(event);
    if(matched_fat_GEN->passes(event)) h_XCone_GEN_GenOnly_matched_fat->fill(event);
    else                               h_XCone_GEN_GenOnly_unmatched_fat->fill(event);
    if(isTTbar) h_GenParticles_GenOnly->fill(event);
    h_RecGenHists_GenOnly->fill(event);
    // fill this only if event passes at least one reco sideband or measurement phase space
    if(pass_measurement_rec || pass_pt350migration_rec || pass_massmigration_rec || pass_btagmigration_rec || pass_subptmigration_rec || pass_leptonptmigration_rec){
      h_RecGenHists_GenSelRecInfo->fill(event);
      if(lowPU)       h_RecGenHists_GenSelRecInfo_lowPU->fill(event);
      else if(midPU)  h_RecGenHists_GenSelRecInfo_midPU->fill(event);
      else if(highPU) h_RecGenHists_GenSelRecInfo_highPU->fill(event);
      if(matched_sub_GEN->passes(event)){
        h_RecGenHists_GenSelRecInfo_matched->fill(event);
        if(lowPU)       h_RecGenHists_GenSelRecInfo_matched_lowPU->fill(event);
        else if(midPU)  h_RecGenHists_GenSelRecInfo_matched_midPU->fill(event);
        else if(highPU) h_RecGenHists_GenSelRecInfo_matched_highPU->fill(event);
      }
    }
  }


  /*************************** fill hists with reco+gen selection applied **************************************************************************/
  if(pass_measurement_rec && pass_measurement_gen){
    h_XCone_GEN_Both->fill(event);
    if(isTTbar) h_GenParticles_Both->fill(event);
    h_RecGenHists_Both->fill(event);
    h_RecGenHists_Both_corr->fill(event);
  }


  /*************************** also store factor from ttbar reweighting ****************************************************************************/
  double ttfactor = 1.0;
  if(isTTbar){
    double weight_before = event.weight;
    ttbar_reweight->process(event);
    double weight_after = event.weight;
    ttfactor = weight_after / weight_before;
  }

  /*************************** write bools for passing selections **********************************************************************************/

  event.set(h_ttbar, isTTbar);
  event.set(h_matched, is_matched_sub);
  event.set(h_ttbar_SF, SF_tt);
  event.set(h_genweight, gen_weight);
  event.set(h_recweight, rec_weight);
  event.set(h_genweight_ttfactor, ttfactor);
  event.set(h_gensel23, passed_gensel23);

  event.set(h_measure_rec, pass_measurement_rec);
  event.set(h_measure_gen, pass_measurement_gen);
  event.set(h_pt350_rec, pass_pt350migration_rec);
  event.set(h_pt350_gen, pass_pt350migration_gen);
  event.set(h_nomass_rec, pass_massmigration_rec);
  event.set(h_nomass_gen, pass_massmigration_gen);
  event.set(h_nobtag_rec, pass_btagmigration_rec);
  event.set(h_ptsub_rec, pass_subptmigration_rec);
  event.set(h_ptsub_gen, pass_subptmigration_gen);
  event.set(h_ptlepton_rec, pass_leptonptmigration_rec);
  event.set(h_ptlepton_gen, pass_leptonptmigration_gen);

  event.set(h_factor_2width, factor_2w);
  event.set(h_factor_4width, factor_4w);
  event.set(h_factor_8width, factor_8w);

  event.set(h_pdf_weights, pdf_weights);

  /*************************** only store events that survive one of the selections (use looser pt cut) ****************************************************************/
  bool in_migrationmatrix = ( pass_measurement_rec ||
    pass_measurement_gen ||
    pass_pt350migration_rec ||
    pass_pt350migration_gen ||
    pass_massmigration_rec ||
    pass_massmigration_gen ||
    pass_btagmigration_rec ||
    pass_subptmigration_rec ||
    pass_subptmigration_gen ||
    pass_leptonptmigration_rec ||
    pass_leptonptmigration_gen   );

    if(!in_migrationmatrix) return false;
    else return true;

  }

  UHH2_REGISTER_ANALYSIS_MODULE(MTopJetPostSelectionModule)
