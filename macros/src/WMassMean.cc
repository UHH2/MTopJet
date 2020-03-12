#include "../include/CentralInclude.h"


using namespace std;
TH1F* GetRatio(TH1F* h1, TH1F* h2);


int main(int argc, char* argv[]){
  TFile* file_mc = new TFile(dir_combine+"uhh2.AnalysisModuleRunner.MC.TTbar.root");
  TFile* file_data = new TFile(dir_combine+"uhh2.AnalysisModuleRunner.DATA.DATA.root");
  TFile* file_jecup = new TFile(dir_combine+"uhh2.AnalysisModuleRunner.MC.TTbar_JECup.root");
  TFile* file_jecdown = new TFile(dir_combine+"uhh2.AnalysisModuleRunner.MC.TTbar_JECdown.root");
  TFile* file_corup = new TFile(dir_combine+"uhh2.AnalysisModuleRunner.MC.TTbar_CORup.root");
  TFile* file_cordown = new TFile(dir_combine+"uhh2.AnalysisModuleRunner.MC.TTbar_CORdown.root");

  TString histname = "XCone_cor_subjets/min_mass_Wjet_zoom";

  vector<TH1F*> hists;

  hists.push_back((TH1F*)file_data->Get(histname));
  hists.push_back((TH1F*)file_mc->Get(histname));
  hists.push_back((TH1F*)file_jecup->Get(histname));
  hists.push_back((TH1F*)file_jecdown->Get(histname));
  hists.push_back((TH1F*)file_corup->Get(histname));
  hists.push_back((TH1F*)file_cordown->Get(histname));

  vector<double> mean, error;
  for(auto h: hists){
    h->GetXaxis()->SetRangeUser(60, 100);
    mean.push_back(h->GetMean());
    error.push_back(h->GetMeanError());
  }

  double jecup = fabs(mean[1] - mean[2]);
  double jecdown = fabs(mean[1] - mean[3]);
  double corup = fabs(mean[1] - mean[4]);
  double cordown = fabs(mean[1] - mean[5]);

  double jecaverage = (jecup+jecdown)/2;
  double coraverage = (corup+cordown)/2;

  cout << "Mean data = " << mean[0] << " +- " << error[0] << endl;
  cout << "Mean MC   = " << mean[1] << " +- " << error[1] << " (stat) +-";
  cout << " +- " << jecaverage << " (jec) +- "<< coraverage << " (cor)" << endl;

  //// ---------------------------------------------------------------------------------------------------------------------
  //// ---------------------------------------------------------------------------------------------------------------------
  return 0;
}
