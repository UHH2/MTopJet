#include "../include/CentralInclude.h"


using namespace std;
TH1F* GetRatio(TH1F* h1, TH1F* h2);


int main(int argc, char* argv[]){

  TString dir = "/nfs/dust/cms/user/schwarzd/MTopJet/PostSelection/muon/";
  vector<TString> processes = {"DATA.DATA", "MC.TTbar", "MC.SingleTop", "MC.DY", "MC.QCD", "MC.DiBoson", "MC.WJets"};
  vector<TString> histdirs = {"PreSel01_Muon", "PreSel02_Muon", "PreSel03_Muon", "PreSel03b_Muon", "PreSel04_Muon", "XCone_cor"};
  TString histname = "number";

  for( auto histdir: histdirs){
    if(histdir=="XCone_cor") histname="number_hadjet";
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
