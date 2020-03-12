#include "../include/CentralInclude.h"


using namespace std;


int main(int argc, char* argv[]){

  //// ---------------------------------------------------------------------------------------------------------------------
  //// ---------------------------- declare files --------------------------------------------------------------------------
  //// ---------------------------------------------------------------------------------------------------------------------
  TFile *f1 = new TFile(dir+"uhh2.AnalysisModuleRunner.MC.TTbar.root");
  TFile *f2 = new TFile("/nfs/dust/cms/user/paaschal/MTopJet/PostSel/2016/muon/uhh2.AnalysisModuleRunner.MC.TTbar_Mtt.root");

  //// ---------------------------------------------------------------------------------------------------------------------
  //// ---------------------------------------------------------------------------------------------------------------------
  TH1F* h1 = (TH1F*)f1->Get( "XCone_cor/M_jet1_");
  TH1F* h2 = (TH1F*)f2->Get( "XCone_cor/M_jet1_");

  h1->Scale(1/h1->Integral());
  h2->Scale(1/h2->Integral());


  gStyle->SetOptStat(kFALSE);
  gStyle->SetPadTickY(1);
  gStyle->SetPadTickX(1);
  gStyle->SetLegendBorderSize(0);


  TCanvas *a = new TCanvas("a", " ", 600, 600);
  gPad->SetLeftMargin(0.15);
  gPad->SetBottomMargin(0.1);
  h1->SetTitle(" ");
  h1->GetXaxis()->SetTitle("m_{jet} [GeV]");
  h1->GetYaxis()->SetTitle("a.u.");
  h1->GetYaxis()->SetRangeUser(0, h2->GetMaximum()*1.1);
  h1->GetYaxis()->SetTitleSize(0.06);
  h1->GetXaxis()->SetTitleSize(0.05);
  h1->GetZaxis()->SetTitleSize(0.05);
  h1->GetXaxis()->SetTitleOffset(0.9);
  h1->GetYaxis()->SetTitleOffset(1.1);
  h1->GetZaxis()->SetTitleOffset(0.9);
  h1->GetXaxis()->SetNdivisions(505);
  h1->GetYaxis()->SetNdivisions(505);
  h1->SetLineColor(kRed);
  h2->SetLineColor(kBlue);
  h1->SetLineWidth(3);
  h2->SetLineWidth(3);
  h2->SetLineStyle(2);
  h1->Draw("HIST");
  h2->Draw("HIST SAME");
  TLegend * leg = new TLegend(0.5, 0.6, 0.85, 0.85);
  leg->AddEntry(h1, "2016v2 t#bar{t} MC", "l");
  leg->AddEntry(h2, "2016v3 t#bar{t} MC", "l");
  leg->SetTextSize(0.04);
  leg->Draw();
  a->SaveAs("/afs/desy.de/user/s/schwarzd/Plots/CompareToAlex.pdf");

  //// ---------------------------------------------------------------------------------------------------------------------
  //// ---------------------------------------------------------------------------------------------------------------------
  return 0;
}
