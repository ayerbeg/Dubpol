#include <sstream>
#include <fstream>
using namespace std;

#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "Variables.hh"


#define READBOOL(myx) \
				if (buf == #myx) \
				{ \
					ss >> wert; \
					myx = ReadBoolean(wert); \
				}

#define READDOUBLE(x,unit) \
				if (buf == #x) \
				{ \
					ss >> doublewert; \
					x = doublewert * unit; \
				}

#define READINT(x) \
				if (buf == #x) \
				{ \
					ss >> intwert; \
					x = intwert; \
				}

#define READSTRING(x) \
				if (buf == #x) \
				{ \
					ss >> x; \
				}

#define READVECTOR(v) \
				if (buf == #v) \
				{ \
					G4double x,y,z; \
					if (ss >> x >> y >> z) \
					v = G4ThreeVector(x, y, z); \
				}



Variables::Variables()
{
	G4cout << "<Variables::Variables>: Will initialize." << G4endl;

// HERE CAN BE SET SOME DEFAULT PARAMETERS IN CASE SBS.ini DO NOT EXISTS OR IT IS INCOMPLETE
// I LEAVE THIS PART OF THE CODE AS EXAMPLE (TO BE REMOVED!!!)

	/*

	mamiEnergie = 1500 * MeV;
	Strahlwinkel = 0;

	Elektrondetektor = "SpecB";
	SpecBWinkel = 40 * deg;
	SpecBImpuls = 300 * MeV;

	Target = "C12";

	UseKolli = false;
	UseJoch = true;
	UseDipol = false;
	UseFibers = true;
	UseFieldMap = true;
	
	Wedeln = false;
	LongTarget = false;
	MaxStepLength = 10*m;
//	LimitStep = false;
	doDecay = false;
	doAllBackround = false;
	doSecondarys = true;
	doEMPhysics = true;
	checkTargethit = true;
	coincOnly = false;
	FastMode = false;
	doDrawTracks = false;
		
	FEEPosition = G4ThreeVector(-1*m, 0, 3*m);
	FEELaenge = 1.4 * m;
	FEEWinkel = 0;

	Detektorabstand = 1.4 * m;
	Detektorwinkel = 0 *  deg;
//	JochVerschiebung;
	Detektorhub = 0;
	Feldstaerke = 1 * tesla;
	DetektorTarget = Detektorabstand;
	
	SciFiWinkel=60*deg;
	FEE_OOP=0*mm;
	
	ESchritte = 20;
	xwSchritte = 5;
	ywSchritte= 5;

	xwWeite=10*deg;
	ywWeite=0.5*deg;
		
	nK = 0;
	doDraw = false;
	isInKField = false;
	GunMode = GM_OTHER;
	TargetX=0;
	TargetY=0;	

	*/

// This code is useful to add a suffix to the data name

	char buffer[100];
	time_t Now = time(0);	
	strftime((char*)&buffer,100,"%Y-%m-%d-%H-%M-%S",localtime(&Now));
	FileNameSuffix = buffer;
	
	
	
}

Variables::~Variables()
{
	G4cout << "<Variables::~Variables>: Finished" << G4endl;
}

G4int 
Variables::LoadFromFile(G4String FileName)
{
    ifstream infile;
    G4String line;
    infile.open(FileName);
    if (infile)
    {
	while (!infile.eof())
	{
	    if (getline(infile,line))
	      {
		G4String buf;
		G4String wert;
		G4double doublewert;
		G4int intwert;
		stringstream ss(line);
		//				G4cout << ">" << line << G4endl;
		ss >> buf;
		//				G4cout << ">" << ss.str() << "<" << G4endl;

		READBOOL(G4GUI);//GUI on/off
		READSTRING(RootFile);

		READSTRING(BeamParticle);
		READDOUBLE(BeamMomentum,GeV);
		
		READINT(NoOfTiles);
		READDOUBLE(TargetShift,mm);

		READDOUBLE(TilesSizeX,mm);
		READDOUBLE(TilesSizeY,mm);
		READDOUBLE(TilesSizeThick,mm);
				
		READSTRING(TargetMat);



		READINT(NoOfCol);
		READINT(NoOfRow);
		
		READINT(NoOfPa);
		READDOUBLE(CalShift,mm);

		READDOUBLE(ModuleSizeX,mm);
		READDOUBLE(ModuleSizeY,mm);
		READDOUBLE(AbsThick,mm);
		READDOUBLE(SciThick,mm);
				
			
		
		/*
		  READDOUBLE(LeadThickness,mm);
		  READDOUBLE(ScintThickness,mm);
		  READINT   (NbOfLayers);
		  
		READDOUBLE(CalModSizeXY,cm);
		
		READINT   (NbOfCaloColumns);
		READINT   (NbOfCaloRows);
		
		READBOOL(ECalDetail);
		READBOOL(Fibers);
		
		READINT (Verbose);//Verbosity
		
		READDOUBLE(EleAng,deg);
		READDOUBLE(ECalDis,m);
		
		READVECTOR(Target);
		

		READINT(ReactionCase);
		READDOUBLE(EnergyBeam,MeV);
		*/
		nK=0;
		if (buf == "Reaction")
		  {
		    G4String Particles;
		    while (!ss.eof())
		      {
			if (ss >> Particles)
			  {
			    ReactionParticle[nK] = Particles;  
			    G4cout << nK << ". Particle:" 
				   << ReactionParticle[nK] << G4endl;
			    nK++;
			  }
		      }
		  } // if Reaktion
		
		
	      } // if getline
	} // if not end of file
    }    // if infile
    else
      {
	G4cerr << "Konnte Datei nicht zum lesen öffnen!" << G4endl;
	return -1;
      } // if infile else
    
    return 0;    
}



// I will not save the variables for the moment
/*
G4int 
SBSVariables::SaveToFile(G4String FileName)
{
	ofstream outfile;
	outfile.open(FileName);
	if (outfile)
	{
		outfile << *this;
		G4cout << "Folgendes wurde ins File geschrieben:" << G4endl << *this;
		outfile.close();
		return 0;
	}
	else
	{
		return -1;
	}
}
*/
ostream & operator << (ostream & os, const Variables v) {
	return os 
	     << v.NbOfCaloColumns << endl
	     << v.NbOfCaloRows << endl
// Meanwhile it is not well implemented how to read data out of the string
// we send only the numbers
//	    << "NbOfCaloColumns " << v.NbOfCaloColumns << endl
//	    << "NbOfCaloRows " << v.NbOfCaloRows << endl

//		<< "Elektrondetektor " << v.Elektrondetektor << endl
//		<< "SpecBWinkel " << v.SpecBWinkel / deg << endl
//		<< "SpecBImpuls " << v.SpecBImpuls / MeV << endl
//		<< "Target " << v.Target << endl
//		<< "TargetX " << v.TargetX/mm << endl
//		<< "TargetY " << v.TargetY/mm << endl
//		<< "UseJoch " << v.UseJoch << endl
//		<< "UseDipol " << v.UseDipol << endl
//		<< "UseFibers " << v.UseFibers << endl
//		<< "UseFieldMap " << v.UseFieldMap << endl
//		<< "Wedeln " << v.Wedeln << endl
//		<< "LongTarget " << v.LongTarget << endl
//		<< "MaxStepLength " << v.MaxStepLength << endl
//		<< "LimitStep " << v.LimitStep << endl
//		<< "doDecay " << v.doDecay << endl
//		<< "doAllBackround " << v.doAllBackround << endl
//		<< "doSecondarys " << v.doSecondarys << endl
//		<< "doEMPhysics " << v.doEMPhysics << endl
//		<< "checkTargethit " << v.checkTargethit << endl
//		<< "coincOnly " << v.coincOnly << endl
//		<< "FastMode " << v.FastMode << endl
//		<< "doDrawTracks " << v.doDrawTracks << endl
//		<< "FEEWinkel " << v.FEEWinkel << endl
//		<< "FEEPosition " << v.FEEPosition.x() / m << " "
//		<< v.FEEPosition.y() / m <<  " " << v.FEEPosition.z() / m << endl
//		<< "FEELaenge " << v.FEELaenge / m << endl
//		<< "Detektorabstand " << v.Detektorabstand / m << endl
//		<< "Detektorwinkel " << v.Detektorwinkel / deg << endl
//		<< "Detektorhub " << v.Detektorhub / mm << endl
//		<< "Feldstaerke " << v.Feldstaerke / tesla << endl
//		<< "SciFiWinkel " << v.SciFiWinkel / deg << endl
//		<< "FEE_OOP " << v.FEE_OOP / mm << endl
//		<< "ESchritte " << v.ESchritte << endl
//		<< "xwSchritte " << v.xwSchritte << endl
//		<< "ywSchritte " << v.ywSchritte << endl
//		<< "xwWeite " << v.xwWeite / deg << endl
//		<< "ywWeite " << v.xwWeite / deg << endl
;
}
/*
ostream & operator << (ostream & os, const SBSVariables *v) {
	return os 
		<< "mamiEnergie " << v->mamiEnergie / MeV << endl
		<< "Strahlwinkel " << v->Strahlwinkel / deg << endl
		<< "Elektrondetektor " << v->Elektrondetektor << endl
		<< "SpecBWinkel " << v->SpecBWinkel / deg << endl
		<< "SpecBImpuls " << v->SpecBImpuls / MeV << endl
		<< "Target " << v->Target << endl
		<< "TargetX " << v->TargetX/mm << endl
		<< "TargetY " << v->TargetY/mm << endl
		<< "UseKolli " << v->UseKolli << endl
		<< "UseJoch " << v->UseJoch << endl
		<< "UseDipol " << v->UseDipol << endl
		<< "UseFibers " << v->UseFibers << endl
		<< "UseFieldMap " << v->UseFieldMap << endl
		<< "Wedeln " << v->Wedeln << endl
		<< "LongTarget " << v->LongTarget << endl
		<< "MaxStepLength " << v->MaxStepLength << endl
//		<< "LimitStep " << v->LimitStep << endl
		<< "doDecay " << v->doDecay << endl
		<< "doAllBackround " << v->doAllBackround << endl
		<< "doSecondarys " << v->doSecondarys << endl
		<< "doEMPhysics " << v->doEMPhysics << endl
		<< "checkTargethit " << v->checkTargethit << endl
		<< "coincOnly " << v->coincOnly << endl
		<< "FastMode " << v->FastMode << endl
		<< "doDrawTracks " << v->doDrawTracks << endl
		<< "FEEWinkel " << v->FEEWinkel << endl
		<< "FEEPosition " << v->FEEPosition.x() / m << " "
		<< v->FEEPosition.y() / m <<  " " << v->FEEPosition.z() / m << endl
		<< "FEELaenge " << v->FEELaenge / m << endl
		<< "Detektorabstand " << v->Detektorabstand / m << endl
		<< "Detektorwinkel " << v->Detektorwinkel / deg << endl
		<< "Detektorhub " << v->Detektorhub / mm << endl
		<< "Feldstaerke " << v->Feldstaerke / tesla << endl
		<< "FEE_OOP " << v->FEE_OOP / mm << endl
		<< "ESchritte " << v->ESchritte << endl
		<< "xwSchritte " << v->xwSchritte << endl
		<< "ywSchritte " << v->ywSchritte << endl
		<< "xwWeite " << v->xwWeite / deg << endl
		<< "ywWeite " << v->xwWeite / deg << endl
;
}
*/



G4bool
Variables::ReadBoolean(G4String Value)
{
    if ((Value=="true")
	|| (Value=="True")
	|| (Value=="1"))
    {return true;}
    else
    {return false;}
}


Variables *iVariables=NULL;
