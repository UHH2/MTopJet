#pragma once
#include "UHH2/core/include/Event.h"
#include "UHH2/common/include/TTbarGen.h"
#include <fastjet/PseudoJet.hh> 
#include <fastjet/JetDefinition.hh>
#include <fastjet/ClusterSequence.hh>
#include <fastjet/ClusterSequenceArea.hh>
#include <fastjet/GhostedAreaSpec.hh>
#include <fastjet/contrib/HOTVR.hh>
#include "fastjet/contrib/ClusteringVetoPlugin.hh"
#include "fastjet/contrib/VariableRPlugin.hh"

#include "UHH2/common/include/Utils.h"
#include "UHH2/core/include/Jet.h"
#include "UHH2/core/include/TopJet.h"
#include "UHH2/core/include/GenJetWithParts.h"
#include "UHH2/core/include/GenParticle.h"
#include "UHH2/core/include/PFParticle.h"
#include "UHH2/common/include/FJet.h"
#include "UHH2/core/include/AnalysisModule.h"

class JetCluster{

 private:

  std::vector<fastjet::PseudoJet> _hadrons;
  bool IsStable(GenParticle* p);
  bool IsLepton(GenParticle* p);
  fastjet::PseudoJet convert_particle(GenParticle* genparticle);
  fastjet::PseudoJet convert_recoparticle(PFParticle* pfparticle);

 public:

  enum E_algorithm { 
    e_ca, 
    e_akt,
    e_kt,
  };

  std::vector<fastjet::PseudoJet> get_genjets(std::vector<GenParticle>* genparts, enum JetCluster::E_algorithm algorithm, double jet_radius, double ptmin);
  //  std::vector<fastjet::PseudoJet> get_hotvr_jets(std::vector<GenParticle>* genparts, enum  JetCluster::E_algorithm algorithm, double rho, double min_r, double max_r, double mu, double theta, double pt_cut);

  std::vector<fastjet::PseudoJet> particle_in;
  std::vector<fastjet::PseudoJet> particle_in2;
  std::vector<fastjet::PseudoJet> get_recojets(std::vector<PFParticle>* pfparts, enum JetCluster::E_algorithm algorithm, double jet_radius, double ptmin);
  std::vector<Jet> convert_pseudojet_to_jet(std::vector<fastjet::PseudoJet> fjet);
  std::vector<fastjet::PseudoJet> substract_lepton(GenParticle* genparts, std::vector<fastjet::PseudoJet> fjets, double jet_radius);

  void write_genjets(uhh2::Event & event, enum  JetCluster::E_algorithm algorithm, double jet_radius, double ptmin);

  std::vector<fastjet::PseudoJet> particle_in_reco;
  fastjet::ClusterSequence* clust_seq;
  fastjet::ClusterSequence* clust_seq2;
  fastjet::ClusterSequence* clust_seq_reco;
  std::vector<fastjet::PseudoJet> new_jets;
  std::vector<fastjet::PseudoJet> new_jets_cleaned;
  std::vector<fastjet::PseudoJet> new_recojets;
  fastjet::JetDefinition *jetdef;
  fastjet::JetDefinition *jetdef_reco;

  //double _rho, _mu, _theta, _rmin, _rmax, _ptmin,_radius,_pt_cut;
  string _clustering_algorithmus;

};


class JetProducer: public uhh2::AnalysisModule{
public:

    explicit JetProducer(uhh2::Context & ctx, const std::string & name);
    virtual bool process(uhh2::Event & event) override; 
    
private:
    uhh2::Event::Handle<std::vector<Jet>>h_newjets;

};
