#pragma once
#include "UHH2/core/include/Hists.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/common/include/Utils.h"
#include <math.h>
#include <vector>

#include "TH1F.h"
#include "TH2F.h"
#include <iostream>

using namespace uhh2;

class RecoGenHists_subjets: public uhh2::Hists {
public:
    // use the same constructor arguments as Hists for forwarding:
  RecoGenHists_subjets(uhh2::Context & ctx, const std::string & dirname,  const std::string & type);

    virtual void fill(const uhh2::Event & ev) override;

protected:

    bool findMatch(const Event & event, Jet jet);

    TH1F *MassReso, *PtReso, *WMassReso;
    TH1F *PtReso_1, *PtReso_2, *PtReso_3, *PtReso_4, *PtReso_5, *PtReso_6, *PtReso_7, *PtReso_8, *PtReso_9, *PtReso_10;
    TH1F *PtReso_rec1, *PtReso_rec2, *PtReso_rec3, *PtReso_rec4, *PtReso_rec5, *PtReso_rec6, *PtReso_rec7, *PtReso_rec8, *PtReso_rec9, *PtReso_rec10;
    TH1F *PtRec_1, *PtRec_2, *PtRec_3, *PtRec_4, *PtRec_5, *PtRec_6, *PtRec_7, *PtRec_8, *PtRec_9, *PtRec_10;
    TH1F *area_all, *area_iso;
    TH1F *min_mass_Wjet_rec, *min_mass_Wjet_gen;
    TH1F *MatchedGen_1, *MatchedGen_2, *MatchedGen_3, *MatchedGen_4, *MatchedGen_5, *MatchedGen_6, *MatchedGen_7, *MatchedGen_8, *MatchedGen_9, *MatchedGen_10;
    TH1F *MatchedRec_1, *MatchedRec_2, *MatchedRec_3, *MatchedRec_4, *MatchedRec_5, *MatchedRec_6, *MatchedRec_7, *MatchedRec_8, *MatchedRec_9, *MatchedRec_10;
    TH1F *TotalGen_1, *TotalGen_2, *TotalGen_3, *TotalGen_4, *TotalGen_5, *TotalGen_6, *TotalGen_7, *TotalGen_8, *TotalGen_9, *TotalGen_10;
    TH1F *TotalRec_1, *TotalRec_2, *TotalRec_3, *TotalRec_4, *TotalRec_5, *TotalRec_6, *TotalRec_7, *TotalRec_8, *TotalRec_9, *TotalRec_10;

    uhh2::Event::Handle<std::vector<TopJet>>h_recjets;
    uhh2::Event::Handle<std::vector<TopJet>>h_hadjets;
    uhh2::Event::Handle<std::vector<GenTopJet>>h_genjets;

};
