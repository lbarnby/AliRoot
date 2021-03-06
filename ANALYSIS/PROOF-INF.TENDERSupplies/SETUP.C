


void SETUP()
{
  // Load some ROOT libraries
  CheckLoadLibrary("libCore");
  CheckLoadLibrary("libTree");
  CheckLoadLibrary("libGeom");
  CheckLoadLibrary("libVMC");
  CheckLoadLibrary("libMinuit");
  CheckLoadLibrary("libGui");
  CheckLoadLibrary("libXMLParser");
  
  // Load the AliROOT library
  CheckLoadLibrary("libANALYSIS");
  CheckLoadLibrary("libSTEERBase");
  CheckLoadLibrary("libESD");
  CheckLoadLibrary("libCDB");
  CheckLoadLibrary("libTENDER");
  //Load libs needed for TOF
  CheckLoadLibrary("libProof");
  CheckLoadLibrary("libRAWDatabase");
  CheckLoadLibrary("libSTEER");
  CheckLoadLibrary("libTOFbase");
  CheckLoadLibrary("libT0base");
  CheckLoadLibrary("libT0rec");

  CheckLoadLibrary("libTENDERSupplies");
  
  // Set the include paths
  gROOT->ProcessLine(".include TENDERSupplies/TenderSupplies");
    
  // Set our location, so that other packages can find us
  gSystem->Setenv("TenderSupplies_INCLUDE", "TENDERSupplies/TenderSupplies");
}


Int_t CheckLoadLibrary(const char* library)
{
  // checks if a library is already loaded, if not loads the library
  
  if (strlen(gSystem->GetLibraries(library, "", kFALSE)) > 0)
    return 1;
  
  return gSystem->Load(library);
}
