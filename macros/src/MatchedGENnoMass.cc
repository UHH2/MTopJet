#include "../include/CentralInclude.h"

using namespace std;

int main(int argc, char* argv[]){
  TFile* file = new TFile("/nfs/dust/cms/user/schwarzd/MTopJet/PostSelection/muon/uhh2.AnalysisModuleRunner.MC.TTbar.root");

  vector<TString> dirs = {"XCone_GEN_GenOnly_matched", "XCone_GEN_GenOnly", "XCone_GEN_beforeMass_matched", "XCone_GEN_beforeMass"};
  TString histname = "Mass_HadJet33_C";

  vector<TH1F*> hists;
  for(auto d: dirs){
    hists.push_back((TH1F*)file->Get(d+"/"+histname));
  }

  int bin_lo = hists[0]->GetXaxis()->FindBin(140.);
  int bin_hi = hists[0]->GetXaxis()->FindBin(200.);

  double matched = hists[0]->Integral(bin_lo, bin_hi);
  double total = hists[1]->Integral(bin_lo, bin_hi);
  double matched_noMass = hists[2]->Integral(bin_lo, bin_hi);
  double total_noMass = hists[3]->Integral(bin_lo, bin_hi);

  double percent = matched/total*100;
  double percent_noMass = matched_noMass/total_noMass*100;

  cout << "total          | " << total << endl;
  cout << "matched        | " << matched << " (" << percent <<"%)"<<  endl;
  cout << "total noMass   | " << total_noMass << endl;
  cout << "matched noMass | " << matched_noMass << " (" << percent_noMass <<"%)"<<  endl;


  hists[0]->SetLineColor(kRed);
  hists[1]->SetLineColor(13);
  hists[2]->SetLineColor(kRed);
  hists[3]->SetLineColor(13);

  TCanvas *c1 = new TCanvas();
  hists[0]->Draw("HIST");
  hists[1]->Draw("HIST SAME");
  c1->SaveAs("1.pdf");

  TCanvas *c2 = new TCanvas();
  hists[2]->Draw("HIST");
  hists[3]->Draw("HIST SAME");
  c2->SaveAs("2.pdf");


  return 0;
}
