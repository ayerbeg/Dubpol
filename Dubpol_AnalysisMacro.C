#include <iostream>
#include "string.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "math.h"

void Dubpol_AnalysisMacro(char input[40])
{
//    gStyle->SetOptFit(1111);

    Bool_t VERBOSE = 0;

    gStyle->SetOptStat(0);

    Double_t rad2deg = 180/(4*atan(1));

  //******************************************************************
  //code to improve the color palette
  //from the root web page and color codes from:
  //http://ultrahigh.org/2007/08/20/making-pretty-root-color-palettes/
  //******************************************************************
  Double_t r[5]    = { 0.00, 0.00, 0.87, 1.00, 0.51 }; 
  Double_t g[5]    = { 0.00, 0.81, 1.00, 0.20, 0.00 }; 
  Double_t b[5]    = { 0.51, 1.00, 0.12, 0.00, 0.00 }; 
  Double_t stop[5] = { 0.00, 0.04, 0.41, 0.94, 1.00 }; 
  Int_t FI = TColor::CreateGradientColorTable(5, stop, r, g, b, 255);
  gStyle->SetNumberContours(255); 
  //******************************************************************


 
  //    TFile *infile=new TFile("./files/30M_targetdata_onCH.root");
  //   TFile *infile=new TFile("./files/5M_1SciTile_targetdata_onCH2.root");

   TFile *infile=new TFile(input);

   TH1F *hTarTotalEne  = (TH1F*)infile->Get("2");

//TARGET VARIABLES
  
    Int_t    fNoEve;
    Double_t fEdep;
    Int_t    fTileNo;
    Int_t    fPDGid;
    Double_t fkineEne;
    Int_t    fPid;
    Double_t fTheta;
    Double_t fPhi;

    TTree *TargetTree=(TTree*)infile->Get("Target");
        
    Int_t TargetEntries = TargetTree->GetEntries();
    cout<<"TargetEntries: "<<TargetEntries<<endl;

    TargetTree ->SetBranchAddress("NoEve", &fNoEve);
    TargetTree ->SetBranchAddress("Edep", &fEdep);
    TargetTree ->SetBranchAddress("KineEne", &fkineEne);
    TargetTree ->SetBranchAddress("TileNo", &fTileNo);
    TargetTree ->SetBranchAddress("PDGID", &fPDGid);
    TargetTree ->SetBranchAddress("ParentID", &fPid);
    TargetTree ->SetBranchAddress("Theta", &fTheta);
    TargetTree ->SetBranchAddress("Phi", &fPhi);

//TARGET EVENT VARIABLES
  
    Int_t    tNoEve;
    Double_t tEdep;
    Int_t    tTileNo;
   
    TTree *TargetTileTree=(TTree*)infile->Get("TargetTile");
        
    Int_t TargetTileEntries = TargetTileTree->GetEntries();
    cout<<"TargetTileEntries: "<<TargetTileEntries<<endl;

    TargetTileTree ->SetBranchAddress("NoEve", &tNoEve);
    TargetTileTree ->SetBranchAddress("Edep", &tEdep);
    TargetTileTree ->SetBranchAddress("TileNo", &tTileNo);
  

//CALORIMETER VARIABLES

    Int_t    cNoEve;
    Double_t cX, cY, cZ;
    Double_t ckineEne, cTotalEne;
    Double_t cTheta, cPhi;
    Int_t    cPDGID, cPid;

    TTree *CalTree=(TTree*)infile->Get("Cal");
        
    Int_t CalEntries = CalTree->GetEntries();
    cout<<"CalEntries: "<<CalEntries<<endl;

    CalTree ->SetBranchAddress("NoEve", &cNoEve);
    CalTree ->SetBranchAddress("X", &cX);
    CalTree ->SetBranchAddress("Y", &cY);
    CalTree ->SetBranchAddress("Z", &cZ);
    CalTree ->SetBranchAddress("PDGID", &cPDGID);
    CalTree ->SetBranchAddress("ParentID", &cPid);
    CalTree ->SetBranchAddress("kineEne", &ckineEne);
    CalTree ->SetBranchAddress("TotalEne", &cTotalEne);
    CalTree ->SetBranchAddress("Theta", &cTheta);
    CalTree ->SetBranchAddress("Phi", &cPhi);

    Double_t EdepTile[8];

//    TH1F *hTarTotalEne = new TH1F("hTarTotalEne","Energy",801, 0.5, 800.5);

//    TH1D *hTarPDGID = new TH1D("hTarPDGID","PDGid",1e10, 0, 1e10);
    
    TH1F *hEneTarTile[8];
    for (Int_t i = 0; i < 8 ; i++) 
    {
	hEneTarTile[i]       = new TH1F(Form("hEne%02d", i),Form("Energy deposited in Tile %02d",i), 801, .5, 800.5);
    }


//TARGET EVENT PLOTS
    Double_t EventsEdepTile[1000][8];
    
    for(Int_t i =0;i<TargetTileEntries;i++)
    {
//	    TargetTree->Show(i);
	TargetTileTree->GetEntry(i);
	
//	EventsEdepTile[tNoEve][tTileNo]=tEdep;
	{
	    EdepTile[tTileNo] = tEdep; //the energy deposited in each Target Tile
	    hEneTarTile[tTileNo]->Fill(tEdep);
	}	
    }
  


//CALORIMETER PLOTS
	Int_t nbins = 101;
	Double_t theta_max = .37*rad2deg;
	Double_t phi_min = -3.4*rad2deg
;	Double_t phi_max = 3.4*rad2deg;

	TH2F *hAngCorrTot = new TH2F("hAngCorrTot", "Phi vs Theta (4.5 GeV/c n on 40 cm POLYSTYRENE) Total", nbins, phi_min, phi_max, nbins, 0., theta_max);
	TH2F *hAngCorrPro = new TH2F("hAngCorrPro", "Phi vs Theta (4.5 GeV/c n on 40 cm POLYETHYLENE) Protons", nbins, phi_min, phi_max, nbins, 0., theta_max);
	TH2F *hAngCorrNeu = new TH2F("hAngCorrNeu", "Phi vs Theta (4.5 GeV/c n on 40 cm POLYETHYLENE) Neutrons", nbins, phi_min, phi_max, nbins, 0., theta_max);

	TH2F *hThetaEnePro = new TH2F("hThetaEnePro", "Kin. Energy  vs Theta (Scatter protons) ", nbins*5, 0, theta_max , nbins*5, 0.5, 3800.5);
	TH2F *hThetaEneNeu = new TH2F("hThetaEneNeu", "Kin. Energy  vs Theta (Scatter neutrons)", nbins*5, 0, theta_max , nbins*5, 0.5, 3800.5);

	TH2F *hThetaEnepiM = new TH2F("hThetaEnepiM", "Kin. Energy  vs Theta (pi+ at Calorimeter) ", nbins*5, 0, theta_max , nbins*5, 0.5, 3800.5);
	TH2F *hThetaEnepim = new TH2F("hThetaEnepim", "Kin. Energy  vs Theta (pi- at calorimeter) ", nbins*5, 0, theta_max , nbins*5, 0.5, 3800.5);

	TH1F *hCalSumEne = new TH1F("hCalSumEne","Energy",4501, 0.5, 4501.5);

//	TH2F *hThetaEneSum = new TH2F("hThetaEneSum", "Total Energy  vs Theta ", nbins*10, 0, .37 , nbins*10, 0., 4800);

	TH1D *hCalPDGID = new TH1D("hCalPDGID","PDGid",601, -300.5, 300.5);

	TH1D *hCalMulti = new TH1D("hCalMulti","Multiplicity",10, 0.5, 10.5);
	TH1D *hCalMultiPro = new TH1D("hCalMultiPro","Multiplicity (Protons at cal)",10, 0.5, 10.5);
	TH1D *hCalMultiNeu = new TH1D("hCalMultiNeu","Multiplicity (neutrons at cal)",10, 0.5, 10.5);
	TH1D *hCalMultiPiM = new TH1D("hCalMultiPiM","Multiplicity (pi+ at cal)",10, 0.5, 10.5);
	TH1D *hCalMultiPim = new TH1D("hCalMultiPin","Multiplicity (pi- at cal)",10, 0.5, 10.5);


	Int_t NoParHcal = 0;
	Int_t NoProHcal = 0;
	Int_t NoNeuHcal = 0;
	Double_t EneSumXeve[30000000];
	Double_t Multiplicity[30000000];

	Double_t MultiPro[30000000];
	Double_t MultiNeu[30000000];
	Double_t MultiPiM[30000000];
	Double_t MultiPim[30000000];

	
        // for(Int_t i =0;i<30000000;i++)
	// {
	//     Double_t EneSumXeve[i]=0;
	// }

	for(Int_t i =0;i<CalEntries;i++)
	{

	    if (VERBOSE)
	    {
	    if(i%100000==0) cout<<"CalEntrie: "<<i<<endl;
	    }

	    CalTree->GetEntry(i);
//	    CalTree->Show(i);
	    if (cPhi!=0 && cTheta!=0)//ONLY SCATTER PARTICLES
	    {
		if (cPDGID<2112 || cPDGID>2212)//2112=neutron, 2212=proton; species arriving calorimeter
		{
		    hCalPDGID->Fill(cPDGID);
		    NoParHcal++;
		}
	
		hAngCorrTot->Fill(cPhi*rad2deg, cTheta*rad2deg);
		
		if (cPDGID==2112) //&&ckineEne<3650)//2112=neutron
		{
		    NoNeuHcal++;
		    if( (ckineEne-(cTheta*(-2250))<3650))//cut removing elastic line for neutron
		    {
			hAngCorrNeu->Fill(cPhi*rad2deg, cTheta*rad2deg);
			hThetaEneNeu->Fill(cTheta*rad2deg,ckineEne);
			NoNeuHcal++;
		    }
		    MultiNeu[cNoEve]++;
		}
		
		if (cPDGID==2212)//2212=proton
		{
		    hAngCorrPro->Fill(cPhi*rad2deg, cTheta*rad2deg);
		    NoProHcal++;
		    hThetaEnePro->Fill(cTheta*rad2deg,ckineEne);
		    MultiPro[cNoEve]++;
		}
		
		if (cPDGID==211)//211=pi+
		{
		    hThetaEnepiM->Fill(cTheta*rad2deg,ckineEne);
		    MultiPiM[cNoEve]++;		
		}
		
		if (cPDGID==-211)//211=pi+
		{
		    hThetaEnepim->Fill(cTheta*rad2deg,ckineEne);
		    MultiPim[cNoEve]++;		
		}
		
		if (cPDGID==-211||cPDGID==211||cPDGID==2212||cPDGID==2112)//or protons, or neutrons, or pions
		{
		    EneSumXeve[cNoEve]+=ckineEne;
		    Multiplicity[cNoEve]++;
		}
	    }
	    //	    hCalPDGID->Fill(cPDGID);
	}



	//MULTIPLICITY SELECTION 
	Int_t Entry_1[50000000];//I got the number after I run the macro with 30M dimmension
	                      // it helps to reduce the time consuming. CHANGE with other data run
                              // this is a round number

	Int_t Entry_2[8000000];//I got the number after I run the macro with 30M dimmension

	Int_t Entry_3[1000000];

	Int_t m_1=0;
	Int_t m_2=0;
	Int_t m_3=0;

	for(Int_t i =0;i<30000000;i++)
	{
	    if (VERBOSE)
	    {
	    if(i%100000==0) cout<<"Multiplicity loop: "<<i<<endl;
	    }

	    if(EneSumXeve[i]>0.0) hCalSumEne->Fill(EneSumXeve[i]);
	    
	    hCalMulti->Fill(Multiplicity[i]);
	    hCalMultiPro->Fill(MultiPro[i]);
	    hCalMultiNeu->Fill(MultiNeu[i]);
	    hCalMultiPiM->Fill(MultiPiM[i]);
	    hCalMultiPim->Fill(MultiPim[i]);
	    
	    if (Multiplicity[i]==1)
	    {		
		Entry_1[m_1]=i;
		m_1++;
	    }	
	    
	    if (Multiplicity[i]==2)
	    {		
		Entry_2[m_2]=i;
		m_2++;
	    }	

	    if (Multiplicity[i]==3)
	    {		
		Entry_3[m_3]=i;
		m_3++;
	    }	
	    
	    
	}
	
	cout<<"m_1: "<<m_1<<endl;
	cout<<"m_2: "<<m_2<<endl;
	cout<<"m_3: "<<m_3<<endl;

	//MULTIPLICITY SELECTION (END)


//Multiplicity Analysis

//MULTIPLICITY 1
	TH2F *hAngCorrTotMul_1 = new TH2F("hAngCorrTotMul_1", "Phi vs Theta (4.5 GeV/c n on 40 cm POLYSTYRENE) Total", nbins, phi_min, phi_max, nbins, 0., theta_max);
	TH2F *hAngCorrProMul_1 = new TH2F("hAngCorrProMul_1", "Phi vs Theta (4.5 GeV/c n on 40 cm POLYETHYLENE) Protons Mul_1", nbins, phi_min, phi_max, nbins, 0., theta_max);
	TH2F *hAngCorrNeuMul_1 = new TH2F("hAngCorrNeuMul_1", "Phi vs Theta (4.5 GeV/c n on 40 cm POLYETHYLENE) Neutrons Mul_1", nbins, phi_min, phi_max, nbins, 0., theta_max);

	TH2F *hThetaEneProMul_1 = new TH2F("hThetaEneProMul_1", "Kin. Energy  vs Theta (Scatter protons) Mul_1", nbins*5, 0, theta_max , nbins*5, 0.5, 3800.5);
	TH2F *hThetaEneNeuMul_1 = new TH2F("hThetaEneNeuMul_1", "Kin. Energy  vs Theta (Scatter neutrons) Mul_1", nbins*5, 0, theta_max , nbins*5, 0.5, 3800.5);

	TH2F *hThetaEneProMul_1_P0 = new TH2F("hThetaEneProMul_1_P0", "Kin. Energy  vs Theta (Scatter protons) Mul_1 ParentID=0", nbins*5, 0, theta_max , nbins*5, 0.5, 3800.5);
	TH2F *hThetaEneNeuMul_1_P0 = new TH2F("hThetaEneNeuMul_1_P0", "Kin. Energy  vs Theta (Scatter neutrons) Mul_1 ParentID=0", nbins*5, 0, theta_max , nbins*5, 0.5, 3800.5);

	TH2F *hThetaEneProMul_1_P1 = new TH2F("hThetaEneProMul_1_P1", "Kin. Energy  vs Theta (Scatter protons) Mul_1 ParentID=1", nbins*5, 0, theta_max , nbins*5, 0.5, 3800.5);
	TH2F *hThetaEneNeuMul_1_P1 = new TH2F("hThetaEneNeuMul_1_P1", "Kin. Energy  vs Theta (Scatter neutrons) Mul_1 ParentID=1", nbins*5, 0, theta_max , nbins*5, 0.5, 3800.5);

	TH2F *hThetaEneProMul_1_P2 = new TH2F("hThetaEneProMul_1_P2", "Kin. Energy  vs Theta (Scatter protons) Mul_1 ParentID>1", nbins*5, 0, theta_max , nbins*5, 0.5, 3800.5);
	TH2F *hThetaEneNeuMul_1_P2 = new TH2F("hThetaEneNeuMul_1_P2", "Kin. Energy  vs Theta (Scatter neutrons) Mul_1 ParentID>1", nbins*5, 0, theta_max , nbins*5, 0.5, 3800.5);


	TH2F *hThetaEnepiMMul_1 = new TH2F("hThetaEnepiMMul_1", "Kin. Energy  vs Theta (pi+ at Calorimeter) Mul_1", nbins*5, 0, theta_max , nbins*5, 0.5, 3800.5);
	TH2F *hThetaEnepimMul_1 = new TH2F("hThetaEnepimMul_1", "Kin. Energy  vs Theta (pi- at calorimeter) Mul_1", nbins*5, 0, theta_max , nbins*5, 0.5, 3800.5);

	Int_t NoProHcalMul_1 = 0;
	Int_t NoNeuHcalMul_1 = 0;


	TH1D *hCalEneProM1 = new TH1D("hCalEneProM1","Energy Protons Multi 1 at cal", nbins*5, 0.5, 3800.5);
	TH1D *hCalEneNeuM1 = new TH1D("hCalEneNeuM1","Energy Neutrons Multi 1 at cal", nbins*5, 0.5, 3800.5);
	TH1D *hCalEnePiM1 = new TH1D("hCalEnePiM1","Energy Pions Multi 1 at cal", nbins*5, 0.5, 3800.5);


//MULTIPLICITY 2
	TH2F *hAngCorrTotMul_2 = new TH2F("hAngCorrTotMul_2", "Phi vs Theta (4.5 GeV/c n on 40 cm POLYSTYRENE) Total", nbins, phi_min, phi_max, nbins, 0., theta_max);
	TH2F *hAngCorrProMul_2 = new TH2F("hAngCorrProMul_2", "Phi vs Theta (4.5 GeV/c n on 40 cm POLYETHYLENE) Protons Mul_2", nbins, phi_min, phi_max, nbins, 0., theta_max);
	TH2F *hAngCorrNeuMul_2 = new TH2F("hAngCorrNeuMul_2", "Phi vs Theta (4.5 GeV/c n on 40 cm POLYETHYLENE) Neutrons Mul_2", nbins, phi_min, phi_max, nbins, 0., theta_max);

	TH2F *hThetaEneProMul_2 = new TH2F("hThetaEneProMul_2", "Kin. Energy  vs Theta (Scatter protons) Mul_2", nbins*5, 0, theta_max , nbins*5, 0.5, 3800.5);
	TH2F *hThetaEneNeuMul_2 = new TH2F("hThetaEneNeuMul_2", "Kin. Energy  vs Theta (Scatter neutrons) Mul_2", nbins*5, 0, theta_max , nbins*5, 0.5, 3800.5);

	TH2F *hThetaEnepiMMul_2 = new TH2F("hThetaEnepiMMul_2", "Kin. Energy  vs Theta (pi+ at Calorimeter) Mul_2", nbins*5, 0, theta_max , nbins*5, 0.5, 3800.5);
	TH2F *hThetaEnepimMul_2 = new TH2F("hThetaEnepimMul_2", "Kin. Energy  vs Theta (pi- at calorimeter) Mul_2", nbins*5, 0, theta_max , nbins*5, 0.5, 3800.5);

	Int_t NoProHcalMul_2 = 0;
	Int_t NoNeuHcalMul_2 = 0;

	TH1D *hCalEneProM2 = new TH1D("hCalEneProM2","Energy Protons Multi 2 at cal", nbins*5, 0.5, 3800.5);
	TH1D *hCalEneNeuM2 = new TH1D("hCalEneNeuM2","Energy Neutrons Multi 2 at cal", nbins*5, 0.5, 3800.5);
	TH1D *hCalEnePiM2 = new TH1D("hCalEnePiM2","Energy Pions Multi 2 at cal", nbins*5, 0.5, 3800.5);


	TH1D *hCalPDGID_2 = new TH1D("hCalPDGID_2","PDGid neutrons multi 2",601, -300.5, 300.5);


//MULTIPLICITY 2 (with companions)

	TH2F *hTheEne_n_2 = new TH2F("hTheEne_n_2", "Kin. Energy  vs Theta (Scatter neutrons w/o pions) Mul_2", nbins*5, 0, theta_max , nbins*5, 0.5, 3800.5);
	TH2F *hTheEne_p_2 = new TH2F("hTheEne_p_2", "Kin. Energy  vs Theta (Scatter protons w/o pions) Mul_2", nbins*5, 0, theta_max , nbins*5, 0.5, 3800.5);

	TH2F *hTheEne_np_2 = new TH2F("hTheEne_np_2", "Kin. Energy  vs Theta (Scatter neutrons and protons) Mul_2", nbins*5, 0, theta_max , nbins*5, 0.5, 3800.5);
	TH2F *hTheEne_pp_2 = new TH2F("hTheEne_pp_2", "Kin. Energy  vs Theta (Scatter protons and protons) Mul_2", nbins*5, 0, theta_max , nbins*5, 0.5, 3800.5);
	TH2F *hTheEne_nn_2 = new TH2F("hTheEne_nn_2", "Kin. Energy  vs Theta (Scatter neutrons and neutrons) Mul_2", nbins*5, 0, theta_max , nbins*5, 0.5, 3800.5);

	TH2F *hTheEne_npiM_2 = new TH2F("hTheEne_npiM_2", "Kin. Energy  vs Theta (Scatter neutrons and pi+) Mul_2", nbins*5, 0, theta_max , nbins*5, 0.5, 3800.5);
	TH2F *hTheEne_npim_2 = new TH2F("hTheEne_npim_2", "Kin. Energy  vs Theta (Scatter neutrons and pi-) Mul_2", nbins*5, 0, theta_max , nbins*5, 0.5, 3800.5);

	TH2F *hTheEne_ppiM_2 = new TH2F("hTheEne_ppiM_2", "Kin. Energy  vs Theta (Scatter protons and pi+) Mul_2", nbins*5, 0, theta_max , nbins*5, 0.5, 3800.5);
	TH2F *hTheEne_ppim_2 = new TH2F("hTheEne_ppim_2", "Kin. Energy  vs Theta (Scatter neutrons and pi-) Mul_2", nbins*5, 0, theta_max , nbins*5, 0.5, 3800.5);

	Int_t PDGid_2[2];
	Double_t KineEne_2[2];
	Double_t Theta_2[2];

	Int_t PDGid_3[3];
	Double_t KineEne_3[3];
	Double_t Theta_3[3];

	Int_t k_1=0;
	Int_t k_2=0;
	Int_t k_3=0;

	Int_t npi;
	Int_t ppi;
	Int_t nn;
	Int_t np;
	Int_t pp;

	/*
//MULTIPLICITY 3 (with companions) (DO NOT USE IT!! SLOW ANALYSIS)

	TH2F *hTheEne_npipi_3 = new TH2F("hTheEne_npipi_3", "Kin. Energy  vs Theta (Scatter neutron and pi+pi) Mul_3", nbins*5, 0, theta_max , nbins*5, 0.5, 3800.5);
	TH2F *hTheEne_ppipi_3 = new TH2F("hTheEne_ppipi_3", "Kin. Energy  vs Theta (Scatter proton and pi+pi) Mul_3", nbins*5, 0, theta_max , nbins*5, 0.5, 3800.5);
	TH2F *hTheEne_nnpi_3 = new TH2F("hTheEne_nnpi_3", "Kin. Energy  vs Theta (Scatter neutron and neutron +pi) Mul_3", nbins*5, 0, theta_max , nbins*5, 0.5, 3800.5);
	TH2F *hTheEne_pppi_3 = new TH2F("hTheEne_pppi_3", "Kin. Energy  vs Theta (Scatter proton and proton +pi) Mul_3", nbins*5, 0, theta_max , nbins*5, 0.5, 3800.5);
	TH2F *hTheEne_pnpi_3 = new TH2F("hTheEne_pnpi_3", "Kin. Energy  vs Theta (Scatter proton and neutron +pi) Mul_3", nbins*5, 0, theta_max , nbins*5, 0.5, 3800.5);
	TH2F *hTheEne_ppn_3 = new TH2F("hTheEne_ppn_3", "Kin. Energy  vs Theta (Scatter proton and proton and neutron) Mul_3", nbins*5, 0, theta_max , nbins*5, 0.5, 3800.5);
	TH2F *hTheEne_pnn_3 = new TH2F("hTheEne_pnn_3", "Kin. Energy  vs Theta (Scatter proton and neutron and neutron) Mul_3", nbins*5, 0, theta_max , nbins*5, 0.5, 3800.5);

	TH2F *hTheEne_ppp_3 = new TH2F("hTheEne_ppp_3", "Kin. Energy  vs Theta (Scatter proton and proton and proton) Mul_3", nbins*5, 0, theta_max , nbins*5, 0.5, 3800.5);
	TH2F *hTheEne_nnn_3 = new TH2F("hTheEne_nnn_3", "Kin. Energy  vs Theta (Scatter neutron and neutron and neutron) Mul_3", nbins*5, 0, theta_max , nbins*5, 0.5, 3800.5);
	TH2F *hTheEne_pipipi_3 = new TH2F("hTheEne_pipipi_3", "Kin. Energy  vs Theta (Scatter pi and pi and pi) Mul_3", nbins*5, 0, theta_max , nbins*5, 0.5, 3800.5);

	Int_t npipi;
	Int_t ppipi;
	Int_t nnpi;
	Int_t pppi;
	Int_t pnpi;
	Int_t ppn;
	Int_t pnn;
	Int_t ppp;
	Int_t nnn;
	Int_t pipipi;
*/



	Int_t p=0;
	Int_t NtupleParID_1[30000000];

	for(Int_t i =0;i<CalEntries;i++)
	{
	    if (VERBOSE)
	    {
		if(i%100000==0) cout<<"CalEntry (2): "<<i<<endl;
	    }
	    
	    CalTree->GetEntry(i);
	    
	    if(cNoEve==Entry_1[k_1])//G4 events with multiplicity 1 at calorimeter
	    {
		if (cPid==1)
		{
		    NtupleParID_1[Entry_1[k_1]]=i;//array with the NTuple events of Multiplicity 1 and ParentID 1
		}
		else
		{
		    NtupleParID_1[Entry_1[k_1]]=-1;//array with the NTuple events of Multiplicity 1 BUT parentID !=1
		}
		
		if (cPhi!=0 && cTheta!=0)//removing NON scatter particles
		{
		    if (cPDGID==2112) //&&ckineEne<3650)//2112=neutron
		    {
//			if( (ckineEne-(cTheta*(-2250))<3650))//cut removing elastic line for neutron
			{
			    hCalEneNeuM1->Fill(ckineEne);
			    hAngCorrNeuMul_1->Fill(cPhi*rad2deg, cTheta*rad2deg);
			    hThetaEneNeuMul_1->Fill(cTheta*rad2deg,ckineEne);
			    NoNeuHcalMul_1++;
			}
			if (cPid==0)
			{
			    hThetaEneNeuMul_1_P0->Fill(cTheta*rad2deg,ckineEne);
			}
			
			if (cPid==1)
			{
			    hThetaEneNeuMul_1_P1->Fill(cTheta*rad2deg,ckineEne);
			}
			
			if (cPid>1)
			{
			    hThetaEneNeuMul_1_P2->Fill(cTheta*rad2deg,ckineEne);
			}
			
		    }
		    
		    
		    if (cPDGID==2212)//2212=proton
		    {
			hCalEneProM1->Fill(ckineEne);
			hAngCorrProMul_1->Fill(cPhi*rad2deg, cTheta*rad2deg);
			hThetaEneProMul_1->Fill(cTheta*rad2deg,ckineEne);
			NoProHcalMul_1++;
			if (cPid==0)
			{
			    hThetaEneProMul_1_P0->Fill(cTheta*rad2deg,ckineEne);
			}
			if (cPid==1)
			{
			    hThetaEneProMul_1_P1->Fill(cTheta*rad2deg,ckineEne);
			}
			if (cPid>1)
			{
			    hThetaEneProMul_1_P2->Fill(cTheta*rad2deg,ckineEne);
			}
			
		    }
		    
		    if (cPDGID==211)//211=pi+
		    {
			hCalEnePiM1->Fill(ckineEne);
			hThetaEnepiMMul_1->Fill(cTheta*rad2deg,ckineEne);
		    }
		    
		    if (cPDGID==-211)//211=pi+
		    {
			hCalEnePiM1->Fill(ckineEne);
			hThetaEnepimMul_1->Fill(cTheta*rad2deg,ckineEne);
		    }
		    
		    k_1++;
		}
	    }
	
//**********************************		    		    
//        Multiplicity 2
//**********************************		    		    
	    if(cNoEve==Entry_2[k_2])
	    {
		if (cPhi!=0 && cTheta!=0)//removing NON scatter particles
		{
		    if (cPDGID==2112) //&&ckineEne<3650)//2112=neutron
		    {
			//		    if( (ckineEne-(cTheta*(-2250))<3650))//cut removing elastic line for neutron
			{
			    hCalEneNeuM2->Fill(ckineEne);
			    hAngCorrNeuMul_2->Fill(cPhi*rad2deg, cTheta*rad2deg);
			    hThetaEneNeuMul_2->Fill(cTheta*rad2deg,ckineEne);
			    NoNeuHcalMul_2++;
			}
		    }
		    
		    
		    if (cPDGID==2212)//2212=proton
		    {
			hCalEneProM2->Fill(ckineEne);
			hAngCorrProMul_2->Fill(cPhi*rad2deg, cTheta*rad2deg);
			hThetaEneProMul_2->Fill(cTheta*rad2deg,ckineEne);
			NoProHcalMul_2++;
		    }
		    
		    if (cPDGID==211)//211=pi+
		    {
			hCalEnePiM2->Fill(ckineEne);
			hThetaEnepiMMul_2->Fill(cTheta*rad2deg,ckineEne);
		    }
		    
		    if (cPDGID==-211)//-211=pi-
		    {
			hCalEnePiM2->Fill(ckineEne);
			hThetaEnepimMul_2->Fill(cTheta*rad2deg,ckineEne);
		    }
//**********************************		    		    		    



//**********************************
//    Multiplicity 2 companions
//**********************************		    

		    PDGid_2[0]=cPDGID;
		    KineEne_2[0]=ckineEne;
		    Theta_2[0]=cTheta;
		    
		    // CalTree->Show(i);
		    // CalTree->Show(i+1);
		    
		    CalTree->GetEntry(i+1);
		    
		    PDGid_2[1]=cPDGID;
		    KineEne_2[1]=ckineEne;
		    Theta_2[1]=cTheta;
		    
//Neutron-pion		    
		    if(
			(PDGid_2[0]==2112&&abs(PDGid_2[1])==211)||
			(PDGid_2[1]==2112&&abs(PDGid_2[0])==211)
			)
		    {
			for(Int_t j=0;j<2;j++)
			{
			    hTheEne_npiM_2->Fill(Theta_2[j]*rad2deg,KineEne_2[j]);  
			    
			}
			npi++;
		    }
		    
//proton-pion
		    if(
			(PDGid_2[0]==2212&&abs(PDGid_2[1])==211)||
			(PDGid_2[1]==22112&&abs(PDGid_2[0])==211)
			)
		    {
			for(Int_t j=0;j<2;j++)
			{
			    hTheEne_ppiM_2->Fill(Theta_2[j]*rad2deg,KineEne_2[j]);  
			}
			ppi++;
		    }
//neutron-proton
		    if(
			(PDGid_2[0]==2212&&PDGid_2[1]==2112)||
			(PDGid_2[1]==22112&&PDGid_2[0]==2212)
			)
		    {
			for(Int_t j=0;j<2;j++)
			{
			    hTheEne_np_2->Fill(Theta_2[j]*rad2deg,KineEne_2[j]);  
			}
			np++;
		    }
//proton-proton
		    if(
			(PDGid_2[0]==2212&&PDGid_2[1]==2212)
			)
		    {
			for(Int_t j=0;j<2;j++)
			{
			    hTheEne_pp_2->Fill(Theta_2[j]*rad2deg,KineEne_2[j]);  
			}
			pp++;
		    }
//neutron-neutron
		    if(
			(PDGid_2[0]==2112&&PDGid_2[1]==2112)
			)
		    {
			for(Int_t j=0;j<2;j++)
			{
			    hTheEne_nn_2->Fill(Theta_2[j]*rad2deg,KineEne_2[j]);  
			}
			nn++;
		    }
		    
		    if(Entry_2[k_2+1]!=cNoEve)
		    {
			k_2++;
			
			if (VERBOSE)
			{	
			    if(k_2%100000==0) cout<<k_2<<endl;
			}
			
		    }
		    
		}//CLOSE remoVE NON SCATTER PARTICLES
		
	    }//CLOSE >>>if(cNoEve==Entry_2[k_2])<<<
	}
//**********************************		    		    	    



//**********************************		    		    
//        Multiplicity 3 (NOT USED)
//**********************************		    		    

/*
	    if(cNoEve==Entry_3[k_3])
	    {
		PDGid_3[0]=cPDGID;
		KineEne_3[0]=ckineEne;
		Theta_3[0]=cTheta;
		
		// CalTree->Show(i);
		// CalTree->Show(i+1);
		
		CalTree->GetEntry(i+1);
		
		PDGid_3[1]=cPDGID;
		KineEne_3[1]=ckineEne;
		Theta_3[1]=cTheta;
		    
		CalTree->GetEntry(i+2);
		
		PDGid_3[2]=cPDGID;
		KineEne_3[2]=ckineEne;
		Theta_3[2]=cTheta;	    

//Neutron-pion-pion	
		    if(
			(PDGid_3[0]==2112&&abs(PDGid_3[1])==211&&abs(PDGid_3[2])==211)||
			(PDGid_3[1]==2112&&abs(PDGid_3[0])==211&&abs(PDGid_3[2])==211)||
			(PDGid_3[2]==2112&&abs(PDGid_3[0])==211&&abs(PDGid_3[1])==211)
			)
		    {
			for(Int_t j=0;j<3;j++)
			{
			    hTheEne_npipi_3->Fill(Theta_3[j]*rad2deg,KineEne_3[j]);  
			    
			}
			npipi++;
		    }

//proton-pion-pion
		    if(
			(PDGid_3[0]==2212&&abs(PDGid_3[1])==211&&abs(PDGid_3[2])==211)||
			(PDGid_3[1]==2212&&abs(PDGid_3[0])==211&&abs(PDGid_3[2])==211)||
			(PDGid_3[2]==2212&&abs(PDGid_3[0])==211&&abs(PDGid_3[1])==211)
			)
		    {
			for(Int_t j=0;j<3;j++)
			{
			    hTheEne_ppipi_3->Fill(Theta_3[j]*rad2deg,KineEne_3[j]);  
			    
			}
			ppipi++;
		    }

//neutron-neutron-pion
		    if(
			(PDGid_3[0]==2112&&PDGid_3[1]==2112&&abs(PDGid_3[2])==211)||//n,n,pi
			(PDGid_3[0]==2112&&PDGid_3[2]==2112&&abs(PDGid_3[1])==211)||//n,pi,n
			(PDGid_3[1]==2112&&PDGid_3[2]==2112&&abs(PDGid_3[0])==211)//pi,n,n
			)
		    {
			for(Int_t j=0;j<3;j++)
			{
			    hTheEne_nnpi_3->Fill(Theta_3[j]*rad2deg,KineEne_3[j]);  
			    
			}
			nnpi++;
		    }


//proton-proton-pion
		    if(
			(PDGid_3[0]==2212&&PDGid_3[1]==2212&&abs(PDGid_3[2])==211)||//p,p,pi
			(PDGid_3[0]==2212&&abs(PDGid_3[1])==211&&PDGid_3[2]==2212)||//p,pi,p
			(PDGid_3[1]==2212&&abs(PDGid_3[0])==211&&PDGid_3[2]==2212)//pi,p,p
			)
		    {
			for(Int_t j=0;j<3;j++)
			{
			    hTheEne_pppi_3->Fill(Theta_3[j]*rad2deg,KineEne_3[j]);  
			    
			}
			pppi++;
		    }

//proton-neutron-pion
		    if(
			(PDGid_3[0]==2212&&abs(PDGid_3[2])==211&&PDGid_3[1]==2112)||//p, n, pi
			(PDGid_3[0]==2212&&abs(PDGid_3[1])==211&&PDGid_3[2]==2112)||//p, pi,n
			(PDGid_3[1]==2212&&abs(PDGid_3[0])==211&&PDGid_3[2]==2112)||//pi,p, n
			(PDGid_3[1]==2212&&abs(PDGid_3[2])==211&&PDGid_3[0]==2112)||//n, p, pi
			(PDGid_3[2]==2212&&abs(PDGid_3[0])==211&&PDGid_3[1]==2112)||//pi,p, n
			(PDGid_3[2]==2212&&abs(PDGid_3[1])==211&&PDGid_3[0]==2112)  //n, pi,p
			)
		    {
			for(Int_t j=0;j<3;j++)
			{
			    hTheEne_pnpi_3->Fill(Theta_3[j]*rad2deg,KineEne_3[j]);  
			    
			}
			pnpi++;
		    }

//proton-proton-neutron
		    if(
			(PDGid_3[0]==2212&&PDGid_3[1]==2212&&PDGid_3[2]==2112)||//p, p, n
			(PDGid_3[0]==2212&&PDGid_3[2]==2212&&PDGid_3[1]==2112)||//p, n, p
			(PDGid_3[1]==2212&&PDGid_3[2]==2212&&PDGid_3[0]==2112)//n, p, p
			)
		    {
			for(Int_t j=0;j<3;j++)
			{
			    hTheEne_ppn_3->Fill(Theta_3[j]*rad2deg,KineEne_3[j]);  
			    
			}
			ppn++;
		    }

//proton-neutron-neutron
		    if(
			(PDGid_3[0]==2212&&PDGid_3[1]==2112&&PDGid_3[2]==2112)||//p, n, n
			(PDGid_3[0]==2112&&PDGid_3[2]==2212&&PDGid_3[1]==2112)||//n, n, p
			(PDGid_3[1]==2212&&PDGid_3[2]==2112&&PDGid_3[0]==2112)//n, p, n
			)
		    {
			for(Int_t j=0;j<3;j++)
			{
			    hTheEne_pnn_3->Fill(Theta_3[j]*rad2deg,KineEne_3[j]);  
			    
			}
			pnn++;
		    }

//neutron-neutron-neutron
		    if(
			(PDGid_3[0]==2112&&PDGid_3[1]==2112&&PDGid_3[2]==2112)//n, n, n
			)
		    {
			for(Int_t j=0;j<3;j++)
			{
			    hTheEne_nnn_3->Fill(Theta_3[j]*rad2deg,KineEne_3[j]);  
			    
			}
			nnn++;
		    }

//proton-proton-proton
		    if(
			(PDGid_3[0]==2212&&PDGid_3[1]==2212&&PDGid_3[2]==2212)//n, n, n
			)
		    {
			for(Int_t j=0;j<3;j++)
			{
			    hTheEne_ppp_3->Fill(Theta_3[j]*rad2deg,KineEne_3[j]);  
			    
			}
			ppp++;
		    }

//proton-proton-proton
		    if(
			(abs(PDGid_3[0])==211&&abs(PDGid_3[1])==211&&abs(PDGid_3[2])==211)
			)
		    {
			for(Int_t j=0;j<3;j++)
			{
			    hTheEne_pipipi_3->Fill(Theta_3[j]*rad2deg,KineEne_3[j]);  
			    
			}
			pipipi++;
		    }



		    if(Entry_3[k_3+2]!=cNoEve)
		    {
			k_3++;
			
			if (VERBOSE)
			{	
			    if(k_3%100000==0) cout<<k_3<<endl;
			}
			
		    }
		    
	    }//CLOSE >>>if(cNoEve==Entry_3[k_3])<<<
//**********************************		    		    	    
*/	    


	

//END Multiplicity Analysis


//TARGET ANALYSIS


	TH1D *hEnePi = new TH1D("hEnePi","Pion Energy deposited",801, 0.5, 801.5);
	TH1D *hEnePro = new TH1D("hEnePro","Proton Energy deposited",801, 0.5, 801.5);
	TH1D *hEneNeu = new TH1D("hEneNeu","Neutron Energy deposited)",801, 0.5, 801.5);
	
	TH1D *hThetaPro = new TH1D("hThetaPro","Proton Theta", 1800, 0.5, 180.5 );

	TH1D *hThetaPro3GeV = new TH1D("hThetaPro3GeV","Proton Theta (linear cut (neutrons at cal with multi=1)", 1800, 0.5, 180.5 );

	TH2F *hTheNeuC_TheProT = new TH2F("hTheNeuC_TheProT", "Theta neutron (@Cal - Mul 1) vs Corresponding Theta Proton (@Tar)", 200, 0.5, 20.5, 1800, 0.5, 180.5 );
	TH2F *hTheNeuC_TheProT_Cut3 = new TH2F("hTheNeuC_TheProT_Cut3", "Theta neutron (@Cal - Mul 1) vs Corresponding Theta Proton (@Tar)", 200, 0.5, 20.5, 1800, 0.5, 180.5 );

	TH2F *hKiEnNeuC_KinEneProT = new TH2F("hKiEnNeuC_KinEneProT", "KinEne neutron (@Cal - Mul 1) vs Corresponding KinEne Proton (@Tar - ParentID = 1)",nbins*5, 0.5, 3800.5, nbins*5, 0.5, 3800.5 );
	TH2F *hKiEnNeuC_KinEneProT3GeV = new TH2F("hKiEnNeuC_KinEneProT3GeV", "KinEne neutron>3GeV (@Cal - Mul 1) vs Corresponding KinEne Proton (@Tar - ParentID = 1)",nbins*5, 0.5, 3800.5, nbins*5, 0.5, 3800.5 );
	
	TH2F *hThetaEneNeu_Cut1 = new TH2F("hThetaEneNeu_Cut1", "Kin. Energy  vs Theta (Scatter neutron Mul1) (Theta Proton @ Target>60deg)", nbins*5, 0, theta_max , nbins*5, 0.5, 3800.5);
	TH2F *hThetaEneNeu_Cut2 = new TH2F("hThetaEneNeu_Cut2", "Kin. Energy  vs Theta (Scatter neutron Mul1) (Theta Proton @ Target>14deg AND <57deg)", nbins*5, 0, theta_max , nbins*5, 0.5, 3800.5);
	TH2F *hThetaEneNeu_Cut3 = new TH2F("hThetaEneNeu_Cut3", "Kin. Energy  vs Theta (Scatter neutron Mul1) (Theta Proton @ Target Triangular cut)", nbins*5, 0, theta_max , nbins*5, 0.5, 3800.5);


	Int_t Index=-1;
	Int_t ProRep;	


	Double_t PiEne[50000000];
	Double_t ProEne[50000000];
	Double_t NeuEne[50000000];
	
	for(Int_t i =0;i<TargetEntries;i++)
	{
//	    TargetTree->Show(i);
	    
	    if (i%100000 == 0) 
	    	cout << "\n---> READING event: " << i << endl;
	    
	    TargetTree->GetEntry(i);
	    
	    if (fNoEve==Entry_1[Index+1]) 
	    {
		Index++;
		ProRep = 0;//Here is reinitilize this flag, For every change of G4event
	    }

	    if(Index>-1)
	    {
		if(fNoEve==Entry_1[Index]  &&  NtupleParID_1[Entry_1[Index]]!=-1 )//Condition to read only events with multiplicity 1 at calorimeter
		{
		    {
		      //		if(NtupleParID_1[Entry_1[Index]]!=-1)//I think with this, we cut with events with ParentID 1 at calorimeter (CHECK!!)
			{
			    CalTree->GetEntry(NtupleParID_1[Entry_1[Index]]);
			    
			    if ((cPDGID==2112)&&(fPDGid==2212))//Neutron AT CALORIMETER AND Proton AT Target
			    {
			      ProEne[Index]+=fEdep;
			      
			      if(fPid==1 && ProRep==0)//I guess this is the primary proton in any n-X->p+Y reaction
				{
				  if(fTheta!=0)//no 0's coming into the histogram 
				    {
				      hTheNeuC_TheProT->Fill(cTheta*rad2deg, fTheta*rad2deg); //because the Cal entry selects a neutron, the cTheta corresponds to the neutron
				      
				      if(fTheta*rad2deg>60)
					hThetaEneNeu_Cut1->Fill(cTheta*rad2deg,ckineEne);
				      
				      if(fTheta*rad2deg>14&&fTheta*rad2deg<57)
					hThetaEneNeu_Cut2->Fill(cTheta*rad2deg,ckineEne);
				      
				      if(
					 ( (fTheta*rad2deg>60)&&
					  (fTheta*rad2deg-20*cTheta*rad2deg<40.) )&&
					 (fTheta*rad2deg+4*cTheta*rad2deg<112.)
					 )
					{
					  hTheNeuC_TheProT_Cut3->Fill(cTheta*rad2deg, fTheta*rad2deg);
					  //		hThetaEneNeu_Cut3->Fill(cTheta*rad2deg,ckineEne);
					}
				      
				      hThetaPro->Fill(fTheta*rad2deg);
				      hKiEnNeuC_KinEneProT->Fill(ckineEne, fkineEne);
				      
				      if(450*cTheta*rad2deg+16*ckineEne>56800) 
					{
					  hThetaEneNeu_Cut3->Fill(cTheta*rad2deg,ckineEne);
					  hKiEnNeuC_KinEneProT3GeV->Fill(ckineEne, fkineEne);
					  hThetaPro3GeV->Fill(fTheta*rad2deg);		
					}			    
				    }
				  
				  ProRep++;// With this, we keep that only the first proton of each G4event enters.  
				  // Protons could have more interactions, but with this, we plot the first only
				}
			      
			      
			      if (fPDGid==2112)//Neutron
				{
				  NeuEne[Index]+=fEdep;
				}
			      
			      if (abs(fPDGid)==211)//Pion
				{
				    PiEne[Index]+=fEdep;
				}

			    }
			}
		    }
		}	    
	    }
	    
	}
	
	for(Int_t i =0;i<50000000;i++)
	{
	    if (PiEne[i]!=0)    hEnePi->Fill(PiEne[i]);
	    if (ProEne[i]!=0)   hEnePro->Fill(ProEne[i]);
	    if (NeuEne[i]!=0)   hEneNeu->Fill(NeuEne[i]);
	}		
	


	       



//END TARGET ANALYSIS



	cout<<"*************************************"<<endl;
	cout<<"Particle counting at the Calorimeter"<<endl;
	cout<<"*************************************"<<endl;
	cout<<"Protons     : "<<NoProHcal<<endl;
	cout<<"Neutrons    : "<<NoNeuHcal<<endl;
	cout<<"NO (p or n) : "<<NoParHcal<<endl;

	cout<<"Protons  (mul 1): "<<NoProHcalMul_1<<endl;
	cout<<"Neutrons (mul 1): "<<NoNeuHcalMul_1<<endl;

	cout<<"Protons  (mul 2): "<<NoProHcalMul_2<<endl;
	cout<<"Neutrons (mul 2): "<<NoNeuHcalMul_2<<endl;


	cout<<"n + pi (mul 2): "<<npi<<endl;
	cout<<"p + pi (mul 2): "<<ppi<<endl;
	cout<<"n + p  (mul 2): "<<np<<endl;
	cout<<"n + n  (mul 2): "<<nn<<endl;
	cout<<"p + p  (mul 2): "<<pp<<endl;
	cout<<"*************************************"<<endl;

	// cout<<"n + pi + pi (mul 3): "<<npipi<<endl;
	// cout<<"p + pi + pi (mul 3): "<<ppipi<<endl;

	// cout<<"n + n + pi (mul 3): "<<nnpi<<endl;
	// cout<<"p + p + pi (mul 3): "<<pppi<<endl;
	// cout<<"p + n + pi (mul 3): "<<pnpi<<endl;

	// cout<<"p + p + n (mul 3): "<<ppn<<endl;
	// cout<<"p + n + n (mul 3): "<<pnn<<endl;

	cT1 = new TCanvas("cT1","Energy Target",1);


//	cT1a = new TCanvas("cT1a","PDGid Target",1);


	cT1->cd();
	hTarTotalEne->Draw();

	cT2 = new TCanvas("cT2","Energy per tile (Target)",1400,800); 
	cT2->Divide(4,2);

	for (Int_t i = 0; i < 8 ; i++) 
	{
	    TPad *p1 = cT2->cd(i+1);
	    p1->SetLogy();
	    hEneTarTile[i]->Draw();

	}


//***********************************************
//*************GRAPHS HERE***********************
//***********************************************


//CALORIMETER ENTRANCE PLOTS

	cC1 = new TCanvas("cC1","Phi vs Theta",1);
	cC1->Divide(2,1);
/*
	cC1->cd(1);
	hAngCorrTot->SetXTitle("Phi (deg)"); 
	hAngCorrTot->SetYTitle("Theta (deg)"); 
	hAngCorrTot->Draw("colz");
*/
	cC1->cd(1);
	hAngCorrNeu->SetXTitle("Phi (deg)"); 
	hAngCorrNeu->SetYTitle("Theta (deg)"); 
	hAngCorrNeu->Draw("colz");

	cC1->cd(2);
	hAngCorrPro->SetXTitle("Phi (deg)"); 
	hAngCorrPro->SetYTitle("Theta (deg)"); 
	hAngCorrPro->Draw("colz");

	cC2 = new TCanvas("cC2","PDGid",1);

	cC2->cd();
	hCalPDGID->Draw();

	cC3 = new TCanvas("cC3","Ene vs Theta PRO",1);
	cC3->cd();
	hThetaEnePro->SetXTitle("Theta (deg)"); 
	hThetaEnePro->SetYTitle("Energy (MeV)"); 
	hThetaEnePro->Draw("colz");

	cC4 = new TCanvas("cC4","Ene vs Theta NEU",1);
	cC4->cd();
	hThetaEneNeu->SetXTitle("Theta (deg)"); 
	hThetaEneNeu->SetYTitle("Energy (MeV)"); 
	hThetaEneNeu->Draw("colz");



	cC5 = new TCanvas("cC5","Ene vs Theta (pions)",1);
	cC5->Divide(2,1);

	cC5->cd(1);
	hThetaEnepiM->SetStats(1);
//	hThetaEnepiM->SetTitle("Residual Y (e^{-} 4GeV)");
	hThetaEnepiM->SetXTitle("Theta (deg)"); 
	hThetaEnepiM->SetYTitle("Energy (MeV)"); 
	hThetaEnepiM->Draw("colz");

	cC5->cd(2);
	hThetaEnepim->SetStats(1);
//	hThetaEnepim->SetTitle("");
	hThetaEnepim->SetXTitle("Theta (deg)"); 
	hThetaEnepim->SetYTitle("Energy (MeV)"); 
	hThetaEnepim->Draw("colz");

	cC6 = new TCanvas("cC6","Sum Kin Energy at Cal",1);
	cC6->cd();
	hCalSumEne->Draw();


	cC7 = new TCanvas("cC7","Multiplicity at Cal",1);
	cC7->cd();
	hCalMulti->Draw();

	cC8 = new TCanvas("cC8","Multiplicity at Cal",1);
	cC8->Divide(2,2);


	TPad *p8a = cC8->cd(1);
	p8a->SetLogy();
//	cC8->cd(1);
	hCalMultiNeu->Draw();

	TPad *p8b = cC8->cd(2);
	p8b->SetLogy();
//	cC8->cd(2);
	hCalMultiPro->Draw();

	TPad *p8c = cC8->cd(3);
	p8c->SetLogy();
//	cC8->cd(3);
	hCalMultiPiM->Draw();

	TPad *p8d = cC8->cd(4);
	p8d->SetLogy();
//	cC8->cd(4);
	hCalMultiPim->Draw();


//MULTIPLICITY PLOTS

	cM1_1 = new TCanvas("cM1_1","Phi vs Theta",1);
	cM1_1->Divide(2,1);

	cM1_1->cd(1);
	hAngCorrNeuMul_1->SetXTitle("Phi (deg)"); 
	hAngCorrNeuMul_1->SetYTitle("Theta (deg)"); 
	hAngCorrNeuMul_1->Draw("colz");

	cM1_1->cd(2);
	hAngCorrProMul_1->SetXTitle("Phi (deg)"); 
	hAngCorrProMul_1->SetYTitle("Theta (deg)"); 
	hAngCorrProMul_1->Draw("colz");

	const Double_t min = 1;
	const Double_t max = 100;
	const Int_t nLevels = 999;
	Double_t levels[nLevels];
	
	for(int i = 1; i < nLevels; i++) 
	{
	    levels[i] = min + (max - min) / (nLevels - 1) * (i);
	}
	levels[0] = 1;
	
	cM3_1 = new TCanvas("cM3_1","Ene vs Theta PRO",1);
	cM3_1->Divide(2,1);
	cM3_1->cd(1);

	hThetaEneProMul_1->SetXTitle("Theta (deg)"); 
	hThetaEneProMul_1->SetYTitle("Energy (MeV)"); 
	hThetaEneProMul_1->SetContour((sizeof(levels)/sizeof(Double_t)), levels);
	hThetaEneProMul_1->GetZaxis()->SetRangeUser(min, max);
	hThetaEneProMul_1->GetZaxis()->SetLabelSize(0.03);
	hThetaEneProMul_1->GetYaxis()->SetLabelSize(0.03);
	hThetaEneProMul_1->GetXaxis()->SetLabelSize(0.03);

	hThetaEneProMul_1->Draw("colz");

	cM3_1->cd(2);

	hThetaEneNeuMul_1->SetXTitle("Theta (deg)"); 
	hThetaEneNeuMul_1->SetYTitle("Energy (MeV)"); 
	hThetaEneNeuMul_1->SetContour((sizeof(levels)/sizeof(Double_t)), levels);
	hThetaEneNeuMul_1->GetZaxis()->SetRangeUser(min, max);
	hThetaEneNeuMul_1->GetZaxis()->SetLabelSize(0.03);
	hThetaEneNeuMul_1->GetXaxis()->SetLabelSize(0.03);
	hThetaEneNeuMul_1->GetYaxis()->SetLabelSize(0.03);

	hThetaEneNeuMul_1->Draw("colz");

	cM4_1 = new TCanvas("cM4_1","Ene vs Theta PRO",1);
	cM4_1->Divide(3,2);

	cM4_1->cd(1);

	hThetaEneProMul_1_P0->SetXTitle("Theta (deg)"); 
	hThetaEneProMul_1_P0->SetYTitle("Energy (MeV)"); 
	hThetaEneProMul_1_P0->SetContour((sizeof(levels)/sizeof(Double_t)), levels);
	hThetaEneProMul_1_P0->GetZaxis()->SetRangeUser(min, max);
	hThetaEneProMul_1_P0->GetZaxis()->SetLabelSize(0.03);
	hThetaEneProMul_1_P0->GetYaxis()->SetLabelSize(0.03);
	hThetaEneProMul_1_P0->GetXaxis()->SetLabelSize(0.03);

	hThetaEneProMul_1_P0->Draw("colz");

	cM4_1->cd(2);

	hThetaEneProMul_1_P1->SetXTitle("Theta (deg)"); 
	hThetaEneProMul_1_P1->SetYTitle("Energy (MeV)"); 
	hThetaEneProMul_1_P1->SetContour((sizeof(levels)/sizeof(Double_t)), levels);
	hThetaEneProMul_1_P1->GetZaxis()->SetRangeUser(min, max);
	hThetaEneProMul_1_P1->GetZaxis()->SetLabelSize(0.03);
	hThetaEneProMul_1_P1->GetYaxis()->SetLabelSize(0.03);
	hThetaEneProMul_1_P1->GetXaxis()->SetLabelSize(0.03);

	hThetaEneProMul_1_P1->Draw("colz");

	cM4_1->cd(3);

	hThetaEneProMul_1_P2->SetXTitle("Theta (deg)"); 
	hThetaEneProMul_1_P2->SetYTitle("Energy (MeV)"); 
	hThetaEneProMul_1_P2->SetContour((sizeof(levels)/sizeof(Double_t)), levels);
	hThetaEneProMul_1_P2->GetZaxis()->SetRangeUser(min, max);
	hThetaEneProMul_1_P2->GetZaxis()->SetLabelSize(0.03);
	hThetaEneProMul_1_P2->GetYaxis()->SetLabelSize(0.03);
	hThetaEneProMul_1_P2->GetXaxis()->SetLabelSize(0.03);

	hThetaEneProMul_1_P2->Draw("colz");



	cM4_1->cd(4);

	hThetaEneNeuMul_1_P0->SetXTitle("Theta (deg)"); 
	hThetaEneNeuMul_1_P0->SetYTitle("Energy (MeV)"); 

	hThetaEneNeuMul_1_P0->SetContour((sizeof(levels)/sizeof(Double_t)), levels);
	hThetaEneNeuMul_1_P0->GetZaxis()->SetRangeUser(min, max);
	hThetaEneNeuMul_1_P0->GetZaxis()->SetLabelSize(0.03);
	hThetaEneNeuMul_1_P0->GetXaxis()->SetLabelSize(0.03);
	hThetaEneNeuMul_1_P0->GetYaxis()->SetLabelSize(0.03);

	hThetaEneNeuMul_1_P0->Draw("colz");

	cM4_1->cd(5);

	hThetaEneNeuMul_1_P1->SetXTitle("Theta (deg)"); 
	hThetaEneNeuMul_1_P1->SetYTitle("Energy (MeV)"); 

	hThetaEneNeuMul_1_P1->SetContour((sizeof(levels)/sizeof(Double_t)), levels);
	hThetaEneNeuMul_1_P1->GetZaxis()->SetRangeUser(min, max);
	hThetaEneNeuMul_1_P1->GetZaxis()->SetLabelSize(0.03);
	hThetaEneNeuMul_1_P1->GetXaxis()->SetLabelSize(0.03);
	hThetaEneNeuMul_1_P1->GetYaxis()->SetLabelSize(0.03);

	hThetaEneNeuMul_1_P1->Draw("colz");

	cM4_1->cd(6);

	hThetaEneNeuMul_1_P2->SetXTitle("Theta (deg)"); 
	hThetaEneNeuMul_1_P2->SetYTitle("Energy (MeV)"); 

	hThetaEneNeuMul_1_P2->SetContour((sizeof(levels)/sizeof(Double_t)), levels);
	hThetaEneNeuMul_1_P2->GetZaxis()->SetRangeUser(min, max);
	hThetaEneNeuMul_1_P2->GetZaxis()->SetLabelSize(0.03);
	hThetaEneNeuMul_1_P2->GetXaxis()->SetLabelSize(0.03);
	hThetaEneNeuMul_1_P2->GetYaxis()->SetLabelSize(0.03);

	hThetaEneNeuMul_1_P2->Draw("colz");


	cM5_1 = new TCanvas("cM5_1","Ene vs Theta (pions)",1);
	cM5_1->Divide(2,1);

	cM5_1->cd(1);
	hThetaEnepiMMul_1->SetStats(1);
	hThetaEnepiMMul_1->SetXTitle("Theta (deg)"); 
	hThetaEnepiMMul_1->SetYTitle("Energy (MeV)"); 
	hThetaEnepiMMul_1->Draw("colz");

	cM5_1->cd(2);
	hThetaEnepimMul_1->SetStats(1);
	hThetaEnepimMul_1->SetXTitle("Theta (deg)"); 
	hThetaEnepimMul_1->SetYTitle("Energy (MeV)"); 
	hThetaEnepimMul_1->Draw("colz");


	cM6_1 = new TCanvas("cM6_1","Ene Multi 1",1);
	cM6_1->Divide(1,3);

	cM6_1->cd(1);
	hCalEneNeuM1->SetXTitle("Kin. Energy (MeV)"); 
	hCalEneNeuM1->Draw();

	cM6_1->cd(2);
	hCalEneProM1->SetXTitle("Kin. Energy (MeV)"); 
	hCalEneProM1->Draw();

	cM6_1->cd(3);
	hCalEnePiM1->SetXTitle("Kin. Energy (MeV)"); 
	hCalEnePiM1->Draw();


	cM1_2 = new TCanvas("cM1_2","Phi vs Theta",1);
	cM1_2->Divide(2,1);

	cM1_2->cd(1);
	hAngCorrNeuMul_2->SetXTitle("Phi (deg)"); 
	hAngCorrNeuMul_2->SetYTitle("Theta (deg)"); 
	hAngCorrNeuMul_2->Draw("colz");

	cM1_2->cd(2);
	hAngCorrProMul_2->SetXTitle("Phi (deg)"); 
	hAngCorrProMul_2->SetYTitle("Theta (deg)"); 
	hAngCorrProMul_2->Draw("colz");


	cM3_2 = new TCanvas("cM3_2","Ene vs Theta PRO",1);
	cM3_2->cd();
	hThetaEneProMul_2->SetXTitle("Theta (deg)"); 
	hThetaEneProMul_2->SetYTitle("Energy (MeV)"); 
	hThetaEneProMul_2->Draw("colz");

	cM4_2 = new TCanvas("cM4_2","Ene vs Theta NEU",1);
	cM4_2->cd();
	hThetaEneNeuMul_2->SetXTitle("Theta (deg)"); 
	hThetaEneNeuMul_2->SetYTitle("Energy (MeV)"); 
	hThetaEneNeuMul_2->Draw("colz");



	cM5_2 = new TCanvas("cM5_2","Ene vs Theta (pions)",1);
	cM5_2->Divide(2,1);

	cM5_2->cd(1);
	hThetaEnepiMMul_2->SetStats(1);
	hThetaEnepiMMul_2->SetXTitle("Theta (deg)"); 
	hThetaEnepiMMul_2->SetYTitle("Energy (MeV)"); 
	hThetaEnepiMMul_2->Draw("colz");

	cM5_2->cd(2);
	hThetaEnepimMul_2->SetStats(1);
	hThetaEnepimMul_2->SetXTitle("Theta (deg)"); 
	hThetaEnepimMul_2->SetYTitle("Energy (MeV)"); 
	hThetaEnepimMul_2->Draw("colz");

	cM6_2 = new TCanvas("cM6_2","Prongs",1);
	cM6_2->Divide(3,2);

	cM6_2->cd(1);
	hTheEne_npiM_2->SetStats(1);
	hTheEne_npiM_2->SetXTitle("Theta (deg)"); 
	hTheEne_npiM_2->SetYTitle("Energy (MeV)"); 
	hTheEne_npiM_2->Draw("colz");

	cM6_2->cd(2);
	hTheEne_ppiM_2->SetStats(1);
	hTheEne_ppiM_2->SetXTitle("Theta (deg)"); 
	hTheEne_ppiM_2->SetYTitle("Energy (MeV)"); 
	hTheEne_ppiM_2->Draw("colz");

	cM6_2->cd(3);
	hTheEne_np_2->SetStats(1);
	hTheEne_np_2->SetXTitle("Theta (deg)"); 
	hTheEne_np_2->SetYTitle("Energy (MeV)"); 
	hTheEne_np_2->Draw("colz");
	
	cM6_2->cd(4);
	hTheEne_pp_2->SetStats(1);
	hTheEne_pp_2->SetXTitle("Theta (deg)"); 
	hTheEne_pp_2->SetYTitle("Energy (MeV)"); 
	hTheEne_pp_2->Draw("colz");
	
	cM6_2->cd(5);
	hTheEne_nn_2->SetStats(1);
	hTheEne_nn_2->SetXTitle("Theta (deg)"); 
	hTheEne_nn_2->SetYTitle("Energy (MeV)"); 
	hTheEne_nn_2->Draw("colz");


	cM7_1 = new TCanvas("cM7_1","Ene Multi 2",1);
	cM7_1->Divide(1,3);

	cM7_1->cd(1);
	hCalEneNeuM2->SetXTitle("Kin. Energy (MeV)"); 
	hCalEneNeuM2->Draw();

	cM7_1->cd(2);
	hCalEneProM2->SetXTitle("Kin. Energy (MeV)"); 
	hCalEneProM2->Draw();

	cM7_1->cd(3);
	hCalEnePiM2->SetXTitle("Kin. Energy (MeV)"); 
	hCalEnePiM2->Draw();



	/*
	cM7_3 = new TCanvas("cM7_3","MULTI 3",1);
	cM7_3->Divide(5,2);

	cM7_3->cd(1);
	hTheEne_npipi_3->SetStats(1);
	hTheEne_npipi_3->SetXTitle("Theta (deg)"); 
	hTheEne_npipi_3->SetYTitle("Energy (MeV)"); 
	hTheEne_npipi_3->Draw("colz");

	cM7_3->cd(2);
	hTheEne_ppipi_3->SetStats(1);
	hTheEne_ppipi_3->SetXTitle("Theta (deg)"); 
	hTheEne_ppipi_3->SetYTitle("Energy (MeV)"); 
	hTheEne_ppipi_3->Draw("colz");

	cM7_3->cd(3);
	hTheEne_nnpi_3->SetStats(1);
	hTheEne_nnpi_3->SetXTitle("Theta (deg)"); 
	hTheEne_nnpi_3->SetYTitle("Energy (MeV)"); 
	hTheEne_nnpi_3->Draw("colz");

	cM7_3->cd(4);
	hTheEne_pppi_3->SetStats(1);
	hTheEne_pppi_3->SetXTitle("Theta (deg)"); 
	hTheEne_pppi_3->SetYTitle("Energy (MeV)"); 
	hTheEne_pppi_3->Draw("colz");

	cM7_3->cd(5);
	hTheEne_pnpi_3->SetStats(1);
	hTheEne_pnpi_3->SetXTitle("Theta (deg)"); 
	hTheEne_pnpi_3->SetYTitle("Energy (MeV)"); 
	hTheEne_pnpi_3->Draw("colz");

	cM7_3->cd(6);
	hTheEne_ppn_3->SetStats(1);
	hTheEne_ppn_3->SetXTitle("Theta (deg)"); 
	hTheEne_ppn_3->SetYTitle("Energy (MeV)"); 
	hTheEne_ppn_3->Draw("colz");

	cM7_3->cd(7);
	hTheEne_pnn_3->SetStats(1);
	hTheEne_pnn_3->SetXTitle("Theta (deg)"); 
	hTheEne_pnn_3->SetYTitle("Energy (MeV)"); 
	hTheEne_pnn_3->Draw("colz");

	cM7_3->cd(8);
	hTheEne_ppp_3->SetStats(1);
	hTheEne_ppp_3->SetXTitle("Theta (deg)"); 
	hTheEne_ppp_3->SetYTitle("Energy (MeV)"); 
	hTheEne_ppp_3->Draw("colz");

	cM7_3->cd(9);
	hTheEne_nnn_3->SetStats(1);
	hTheEne_nnn_3->SetXTitle("Theta (deg)"); 
	hTheEne_nnn_3->SetYTitle("Energy (MeV)"); 
	hTheEne_nnn_3->Draw("colz");

	cM7_3->cd(10);
	hTheEne_pipipi_3->SetStats(1);
	hTheEne_pipipi_3->SetXTitle("Theta (deg)"); 
	hTheEne_pipipi_3->SetYTitle("Energy (MeV)"); 
	hTheEne_pipipi_3->Draw("colz");
	*/

	cT1a= new TCanvas("cT1a","Target",1);
	cT1a->Divide(3,1);

	cT1a->cd(1);
	hEnePi->SetXTitle("Energy Deposited (MeV)"); 
	hEnePi->Draw();

	cT1a->cd(2);
	hEnePro->SetXTitle("Energy Deposited (MeV)"); 
	hEnePro->Draw();

	cT1a->cd(3);
	hEneNeu->SetXTitle("Energy Deposited (MeV)"); 
	hEneNeu->Draw();

	cT2a = new TCanvas("cT2a","Target",1);
	cT2a->Divide(1,2);

	cT2a->cd(1);
	hThetaPro->SetXTitle("Theta (deg)"); 
	hThetaPro->Draw();

	cT2a->cd(2);
	hThetaPro3GeV->SetXTitle("Theta (deg)"); 
	hThetaPro3GeV->Draw();

	cT3a = new TCanvas("cT3a","Target",1);

	cT3a->cd();
	hTheNeuC_TheProT->SetXTitle("Neutron Theta (deg)"); 
	hTheNeuC_TheProT->SetYTitle("Proton Theta (deg)"); 
	hTheNeuC_TheProT->Draw("colz");


	cT3aa = new TCanvas("cT3aa","Target",1);


	cT3aa->cd();
	hTheNeuC_TheProT_Cut3->SetXTitle("Neutron Theta (deg)"); 
	hTheNeuC_TheProT_Cut3->SetYTitle("Proton Theta (deg)"); 
	hTheNeuC_TheProT_Cut3->Draw("colz");
//	cT3aa->Print("hTheNeuC_TheProT_Cut3.pdf");
	
	
	cT5a = new TCanvas("cT5a","Target",1);
	cT5a->Divide(2,1);		

	cT5a->cd(1);
	hKiEnNeuC_KinEneProT->SetXTitle("Neutron KinEne (MeV)"); 
	hKiEnNeuC_KinEneProT->SetYTitle("Proton KinEne (MeV)"); 
	hKiEnNeuC_KinEneProT->Draw("colz");

	cT5a->cd(2);
	hKiEnNeuC_KinEneProT3GeV->SetXTitle("Neutron KinEne (MeV)"); 
	hKiEnNeuC_KinEneProT3GeV->SetYTitle("Proton KinEne (MeV)"); 
	hKiEnNeuC_KinEneProT3GeV->Draw("colz");

	cT6a = new TCanvas("cT6a","Target",1);
	cT6a->Divide(3,1);		

	cT6a->cd(1);
	hThetaEneNeu_Cut1->SetStats(1);
	hThetaEneNeu_Cut1->SetXTitle("Theta (deg)"); 
	hThetaEneNeu_Cut1->SetYTitle("Energy (MeV)"); 
	hThetaEneNeu_Cut1->Draw("colz");

	cT6a->cd(2);
	hThetaEneNeu_Cut2->SetStats(1);
	hThetaEneNeu_Cut2->SetXTitle("Theta (deg)"); 
	hThetaEneNeu_Cut2->SetYTitle("Energy (MeV)"); 
	hThetaEneNeu_Cut2->Draw("colz");

	cT6a->cd(3);
	hThetaEneNeu_Cut3->SetStats(1);
	hThetaEneNeu_Cut3->SetXTitle("Theta (deg)"); 
	hThetaEneNeu_Cut3->SetYTitle("Energy (MeV)"); 
	hThetaEneNeu_Cut3->Draw("colz");

//	cT6a->Print("hThetaEneNeu_Cut3.pdf");




}

