/**************************************************************************
 * Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

/* $Id$ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// class for MUON reconstruction                                             //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
#include <TParticle.h>
#include <TArrayF.h>

#include "AliRunLoader.h"
#include "AliHeader.h"
#include "AliGenEventHeader.h"
#include "AliESD.h"

#include "AliMUONData.h"
#include "AliMUONEventReconstructor.h"
#include "AliMUONClusterReconstructor.h"
#include "AliMUONTriggerDecision.h"
#include "AliMUONClusterFinderVS.h"
#include "AliMUONTrack.h"
#include "AliMUONTrackParam.h"
#include "AliMUONTriggerTrack.h"
#include "AliESDMuonTrack.h"
#include "AliMUONReconstructor.h"

ClassImp(AliMUONReconstructor)
//_____________________________________________________________________________
AliMUONReconstructor::AliMUONReconstructor()
  : AliReconstructor()
{
}
//_____________________________________________________________________________
AliMUONReconstructor::~AliMUONReconstructor()
{
}
//_____________________________________________________________________________
void AliMUONReconstructor::Reconstruct(AliRunLoader* runLoader) const
{
//  AliLoader
  AliLoader* loader = runLoader->GetLoader("MUONLoader");
  Int_t nEvents = runLoader->GetNumberOfEvents();

// used local container for each method
// passing fLoader as argument, could be avoided ???
  AliMUONEventReconstructor* recoEvent = new AliMUONEventReconstructor(loader);
  AliMUONData* dataEvent = recoEvent->GetMUONData();

  AliMUONClusterReconstructor* recoCluster = new AliMUONClusterReconstructor(loader);
  AliMUONData* dataCluster = recoCluster->GetMUONData();

  AliMUONTriggerDecision* trigDec = new AliMUONTriggerDecision(loader);
  AliMUONData* dataTrig = trigDec->GetMUONData();


  for (Int_t i = 0; i < 10; i++) {
    AliMUONClusterFinderVS *recModel = new AliMUONClusterFinderVS();
    recModel->SetGhostChi2Cut(10);
    recoCluster->SetReconstructionModel(i,recModel);
  } 

  loader->LoadDigits("READ");
  loader->LoadRecPoints("RECREATE");
  loader->LoadTracks("RECREATE");
  
  //   Loop over events              
  for(Int_t ievent = 0; ievent < nEvents; ievent++) {
    printf("Event %d\n",ievent);
    runLoader->GetEvent(ievent);

    //----------------------- digit2cluster & Digits2Trigger -------------------
    if (!loader->TreeR()) loader->MakeRecPointsContainer();
     
    // tracking branch
    dataCluster->MakeBranch("RC");
    dataCluster->SetTreeAddress("D,RC");
    recoCluster->Digits2Clusters(); 
    dataCluster->Fill("RC"); 

    // trigger branch
    dataTrig->MakeBranch("GLT");
    dataTrig->SetTreeAddress("D,GLT");
    trigDec->Digits2Trigger(); 
    dataTrig->Fill("GLT");

    loader->WriteRecPoints("OVERWRITE");

    //---------------------------- Track & TriggerTrack ---------------------
    if (!loader->TreeT()) loader->MakeTracksContainer();

    // trigger branch
    dataEvent->MakeBranch("RL"); //trigger track
    dataEvent->SetTreeAddress("RL");
    recoEvent->EventReconstructTrigger();
    dataEvent->Fill("RL");

    // tracking branch
    dataEvent->MakeBranch("RT"); //track
    dataEvent->SetTreeAddress("RT");
    recoEvent->EventReconstruct();
    dataEvent->Fill("RT");

    loader->WriteTracks("OVERWRITE");  
  
    //--------------------------- Resetting branches -----------------------
    dataCluster->ResetDigits();
    dataCluster->ResetRawClusters();

    dataTrig->ResetDigits();
    dataTrig->ResetTrigger();

    dataEvent->ResetRawClusters();
    dataEvent->ResetTrigger();
    dataEvent->ResetRecTracks();
    dataEvent->ResetRecTriggerTracks();
  
  }
  loader->UnloadDigits();
  loader->UnloadRecPoints();
  loader->UnloadTracks();

  delete recoCluster;
  delete recoEvent;
  delete trigDec;
}
//_____________________________________________________________________________
void AliMUONReconstructor::FillESD(AliRunLoader* runLoader, AliESD* esd) const
{
  TClonesArray* recTracksArray = 0;
  TClonesArray* recTrigTracksArray = 0;
  
  AliLoader* loader = runLoader->GetLoader("MUONLoader");
  loader->LoadTracks("READ");
  AliMUONData* muonData = new AliMUONData(loader,"MUON","MUON");

   // declaration  
  Int_t iEvent, nPart;
  Int_t nTrackHits, nPrimary;
  Double_t fitFmin;
  TArrayF vertex(3);

  Double_t bendingSlope, nonBendingSlope, inverseBendingMomentum;
  Double_t xRec, yRec, zRec, chi2MatchTrigger;
  Bool_t matchTrigger;

  // setting pointer for tracks, triggertracks & trackparam at vertex
  AliMUONTrack* recTrack = 0;
  AliMUONTrackParam* trackParam = 0;
  AliMUONTriggerTrack* recTriggerTrack = 0;
  TParticle* particle = new TParticle();
  AliGenEventHeader* header = 0;
  iEvent = runLoader->GetEventNumber(); 
  runLoader->GetEvent(iEvent);

  // vertex calculation (maybe it exists already somewhere else)
  vertex[0] = vertex[1] = vertex[2] = 0.;
  nPrimary = 0;
  if ( (header = runLoader->GetHeader()->GenEventHeader()) ) {
    header->PrimaryVertex(vertex);
  } else {
    runLoader->LoadKinematics("READ");
    runLoader->TreeK()->GetBranch("Particles")->SetAddress(&particle);
    nPart = (Int_t)runLoader->TreeK()->GetEntries();
    for(Int_t iPart = 0; iPart < nPart; iPart++) {
      runLoader->TreeK()->GetEvent(iPart);
      if (particle->GetFirstMother() == -1) {
	vertex[0] += particle->Vx();
	vertex[1] += particle->Vy();
	vertex[2] += particle->Vz();
	nPrimary++;
      }
      if (nPrimary) {
	vertex[0] /= (double)nPrimary;
	vertex[1] /= (double)nPrimary;
	vertex[2] /= (double)nPrimary;
      }
    }
  }
  // setting ESD MUON class
  AliESDMuonTrack* theESDTrack = new  AliESDMuonTrack() ;

  //-------------------- trigger tracks-------------
  Long_t trigPat = 0;
  muonData->SetTreeAddress("RL");
  muonData->GetRecTriggerTracks();
  recTrigTracksArray = muonData->RecTriggerTracks();

  // ready global trigger pattern from first track
  if (recTrigTracksArray) 
    recTriggerTrack = (AliMUONTriggerTrack*) recTrigTracksArray->First();
  if (recTriggerTrack) trigPat = recTriggerTrack->GetGTPattern();

  //printf(">>> Event %d Number of Recconstructed tracks %d \n",iEvent, nrectracks);
 
  // -------------------- tracks-------------
  muonData->SetTreeAddress("RT");
  muonData->GetRecTracks();
  recTracksArray = muonData->RecTracks();
        
  Int_t nRecTracks = 0;
  if (recTracksArray)
    nRecTracks = (Int_t) recTracksArray->GetEntriesFast(); //
  
  // loop over tracks
  for (Int_t iRecTracks = 0; iRecTracks <  nRecTracks;  iRecTracks++) {

    // reading info from tracks
    recTrack = (AliMUONTrack*) recTracksArray->At(iRecTracks);

    trackParam = recTrack->GetTrackParamAtVertex();

    bendingSlope            = trackParam->GetBendingSlope();
    nonBendingSlope         = trackParam->GetNonBendingSlope();
    inverseBendingMomentum = trackParam->GetInverseBendingMomentum();
    xRec  = trackParam->GetNonBendingCoor();
    yRec  = trackParam->GetBendingCoor();
    zRec  = trackParam->GetZ();

    nTrackHits       = recTrack->GetNTrackHits();
    fitFmin          = recTrack->GetFitFMin();
    matchTrigger     = recTrack->GetMatchTrigger();
    chi2MatchTrigger = recTrack->GetChi2MatchTrigger();

    // setting data member of ESD MUON
    theESDTrack->SetInverseBendingMomentum(inverseBendingMomentum);
    theESDTrack->SetThetaX(TMath::ATan(nonBendingSlope));
    theESDTrack->SetThetaY(TMath::ATan(bendingSlope));
    theESDTrack->SetZ(vertex[2]);
    theESDTrack->SetBendingCoor(vertex[1]); // calculate vertex at ESD or Tracking level ?
    theESDTrack->SetNonBendingCoor(vertex[0]);
    theESDTrack->SetChi2(fitFmin);
    theESDTrack->SetNHit(nTrackHits);
    theESDTrack->SetMatchTrigger(matchTrigger);
    theESDTrack->SetChi2MatchTrigger(chi2MatchTrigger);

    // storing ESD MUON Track into ESD Event 
    if (nRecTracks != 0)  
      esd->AddMuonTrack(theESDTrack);
  } // end loop tracks

  // add global trigger pattern
  if (nRecTracks != 0)  
    esd->SetTrigger(trigPat);

  // reset muondata
  muonData->ResetRecTracks();
  muonData->ResetRecTriggerTracks();

  //} // end loop on event  
  loader->UnloadTracks(); 
  if (!header)
    runLoader->UnloadKinematics();
  delete theESDTrack;
  delete muonData;
  // delete particle;
}
