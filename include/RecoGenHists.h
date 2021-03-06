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

/**  \brief Example class for booking and filling histograms
 *
 * NOTE: This class uses the 'hist' method to retrieve histograms.
 * This requires a string lookup and is therefore slow if you have
 * many histograms. Therefore, it is recommended to use histogram
 * pointers as member data instead, like in 'common/include/ElectronHists.h'.
 */
class RecoGenHists: public uhh2::Hists {
public:
    // use the same constructor arguments as Hists for forwarding:
  RecoGenHists(uhh2::Context & ctx, const std::string & dirname, const std::string & rec_label, const std::string & gen_label);

    virtual void fill(const uhh2::Event & ev) override;

protected:

    TH1F *MassReso, *PtReso;
    TH1F *DeltaR_Rec1_Gen1, *DeltaR_Rec2_Gen2;
    TH1F *pt_rec1_gen1_beforeMatching, *pt_rec2_gen2_beforeMatching, *pt_rec1_gen1_afterMatching, *pt_rec2_gen2_afterMatching;

    uhh2::Event::Handle<std::vector<Jet>>h_recjets;
    uhh2::Event::Handle<std::vector<Jet>>h_genjets;

};
