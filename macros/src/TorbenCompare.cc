#include "../include/CentralInclude.h"

using namespace std;

int main(int argc, char* argv[]){
  TFile* f_dennis = new TFile("/nfs/dust/cms/user/schwarzd/MTopJet/PostSelection/combine/uhh2.AnalysisModuleRunner.MC.TTbar.root");
  TFile* f_torben = new TFile("/nfs/dust/cms/user/schwarzd/PlotsTorben/root_files/Histograms.TTbar_Matched.root");

  TH1F* gen_dennis = (TH1F*) f_dennis->Get("XCone_GEN_GenOnly_matched/Mass_HadJet33");
  TH1F* gen_torben = (TH1F*) f_torben->Get("Mass_Leading_Gen");

  gen_dennis->Scale(1/gen_dennis->Integral());
  gen_torben->Scale(1/gen_torben->Integral());

  gStyle->SetOptStat(kFALSE);
  gStyle->SetPadTickY(1);
  gStyle->SetPadTickX(1);
  gStyle->SetLegendBorderSize(0);

  TCanvas *c1 = new TCanvas("c1", " ", 600, 600);
  gPad->SetLeftMargin(0.18);
  gPad->SetBottomMargin(0.14);
  gen_dennis->GetXaxis()->SetRangeUser(0, 500);
  // gen_dennis->GetYaxis()->SetRangeUser(0.0, 0.15);
  gen_dennis->GetXaxis()->SetTitle("#it{m}_{jet} [GeV]");
  gen_dennis->GetYaxis()->SetTitle("#frac{1}{#sigma} #frac{d#sigma}{d#it{m}_{jet}}");
  gen_dennis->SetTitle(" ");
  gen_dennis->GetXaxis()->SetNdivisions(505);
  gen_dennis->GetYaxis()->SetNdivisions(505);
  gen_dennis->GetYaxis()->SetTitleOffset(1.5);
  gen_dennis->GetXaxis()->SetTitleOffset(1.1);
  gen_dennis->GetYaxis()->SetTitleSize(0.05);
  gen_dennis->GetXaxis()->SetTitleSize(0.05);
  gen_dennis->GetXaxis()->SetLabelSize(0.05);
  gen_dennis->GetYaxis()->SetLabelSize(0.05);
  gen_dennis->SetLineColor(kAzure+7);
  gen_torben->SetLineColor(kRed-2);
  gen_dennis->SetLineWidth(3);
  gen_torben->SetLineWidth(3);
  gen_dennis->Draw("HIST");
  gen_torben->Draw("HIST SAME");
  TLegend *leg = new TLegend(0.5, 0.42, 0.85, 0.62);
  leg->AddEntry(gen_dennis, "XCone, R_{ }=_{ }1.2", "l");
  leg->AddEntry((TObject*)0, "#scale[0.8]{R_{sub} = 0.4, N_{sub} = 3}", "");
  leg->AddEntry((TObject*)0, "#scale[0.8]{ }", "");
  leg->AddEntry(gen_torben, "CA, R_{ }=_{ }1.2", "l");
  leg->AddEntry((TObject*)0, "#scale[0.6]{[Eur. Phys. J. C 77 (2017) 467]}", "");
  leg->SetTextSize(0.04);
  leg->Draw();
  CMSSimLabel(true, 0.58, 0.85);
  c1->SaveAs("/afs/desy.de/user/s/schwarzd/Plots/TorbenCompare/ParticleLevelComparison.pdf");


  return 0;
}
