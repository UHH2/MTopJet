#include "../include/CentralInclude.h"


using namespace std;
TH1F* GetRatio(TH1F* h1, TH1F* h2);


int main(int argc, char* argv[]){

  TString dir = "/nfs/dust/cms/user/schwarzd/MTopJet/Selection/muon/";
  vector<TString> processes = {"DATA.DATA", "MC.TTbar", "MC.SingleTop", "MC.DY", "MC.QCD", "MC.DiBoson", "MC.WJets"};
  vector<TString> histdirs = {"00_PreSel_Muon", "01_Cleaner_Muon", "03_Lepton_Muon", "04_Jet_Muon", "05_TwoD_Muon", "06_MET_Muon", "07_HTlep_Muon", "08_bTag_Muon"};
  TString histname = "number";

  for( auto histdir: histdirs){
    cout << "------------------------------------" << endl;
    cout << "== " << histdir << endl << endl;
    
    for(auto process: processes){
      TFile* file = new TFile(dir+"uhh2.AnalysisModuleRunner."+process+".root");
      TH1F* hist = (TH1F*) file->Get(histdir+"/"+histname);
      double integral = hist->Integral();
      cout << process << ": " << integral << endl;
    }
    cout << endl;
  }

  return 0;
}
