//____________________________________________________________________
//
// $Id$
//
// Draw hits in the specialised FMD event display 
//
/** Display hits 
    @ingroup FMD_script
 */
void
DisplayHits()
{
  AliCDBManager* cdb = AliCDBManager::Instance();
  cdb->SetDefaultStorage("local://$ALICE_ROOT/OCDB");
  cdb->SetRun(0);
  gSystem->Load("libANALYSIS");
  gSystem->Load("libANALYSISalice");
  gSystem->Load("libFMDanalysis");
  gSystem->Load("libFMDutil");
  AliFMDDisplay* d = new AliFMDDisplay;
  d->AddLoad(AliFMDInput::kHits);
  d->AddLoad(AliFMDInput::kKinematics);
  d->Run();
}

//____________________________________________________________________
//
// EOF
//
