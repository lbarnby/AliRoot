//-*- Mode: C++ -*-
// $Id$
#ifndef ALIHLTTPCDATACOMPRESSIONMONITORCOMPONENT_H
#define ALIHLTTPCDATACOMPRESSIONMONITORCOMPONENT_H
//* This file is property of and copyright by the ALICE HLT Project        * 
//* ALICE Experiment at CERN, All rights reserved.                         *
//* See cxx source for full Copyright notice                               *

/// @file   AliHLTTPCDataCompressionMonitorComponent.h
/// @author Matthias Richter
/// @date   2011-09-12
/// @brief  TPC component for monitoring of data compression
///

#include "AliHLTProcessor.h"
#include "AliHLTTPCRawCluster.h"
#include "AliHLTSpacePointContainer.h"
#include "TString.h"

struct AliHLTTPCClusterMCLabel;
class AliHLTTPCHWCFData;
class AliHLTDataInflater;
class AliHLTTPCTrackGeometry;
class AliHLTComponentBenchmark;
class AliHLTTPCDataCompressionDecoder;
class TH1;
class TH2;
class TH3;

/**
 * @class AliHLTTPCDataCompressionMonitorComponent
 *
 * <h2>General properties:</h2>
 *
 * Component ID: \b TPCDataCompressorMonitor      <br>
 * Library: \b libAliHLTTPC.so     <br>
 * Input Data Types:  <br>
 * Output Data Types: <br>
 *
 * <h2>Mandatory arguments:</h2>
 * <!-- NOTE: ignore the \li. <i> and </i>: it's just doxygen formatting -->
 *
 * <h2>Optional arguments:</h2>
 * <!-- NOTE: ignore the \li. <i> and </i>: it's just doxygen formatting -->

 * <h2>Configuration:</h2>
 * <!-- NOTE: ignore the \li. <i> and </i>: it's just doxygen formatting -->
 *
 * <h2>Default CDB entries:</h2>
 *
 * <h2>Performance:</h2>
 *
 * <h2>Memory consumption:</h2>
 *
 * <h2>Output size:</h2>
 *
 *
 * @ingroup alihlt_tpc
 */
class AliHLTTPCDataCompressionMonitorComponent : public AliHLTProcessor {
public:
  /// standard constructor
  AliHLTTPCDataCompressionMonitorComponent();
  /// destructor
  ~AliHLTTPCDataCompressionMonitorComponent();

  /// inherited from AliHLTComponent: id of the component
  virtual const char* GetComponentID();

  /// inherited from AliHLTComponent: list of data types in the vector reference
  void GetInputDataTypes( AliHLTComponentDataTypeList& );

  /// inherited from AliHLTComponent: output data type of the component.
  AliHLTComponentDataType GetOutputDataType();

  /// inherited from AliHLTComponent: multiple output data types of the component.
  int GetOutputDataTypes(AliHLTComponentDataTypeList& tgtList);

  /// inherited from AliHLTComponent: output data size estimator
  void GetOutputDataSize( unsigned long& constBase, double& inputMultiplier );

  /// inherited from AliHLTComponent: spawn function.
  virtual AliHLTComponent* Spawn();

  enum {
    kHaveRawData = 0x1,
    kHaveHWClusters = 0x2
  };

  enum {
    kPublishOff      = 0,
    kPublishSeparate = 1,
    kPublishList     = 2,
    kPublishArray    = 3,
    kPublishInvalid  = 4    
  };

  enum {
    kHistogramPadrow,
    kHistogramHWCFPad,
    kHistogramPad,
    kHistogramTime,
    kHistogramSigmaY2,
    kHistogramSigmaZ2,
    kHistogramCharge,
    kHistogramQMax,
    kHistogramDeltaPadrow,
    kHistogramDeltaPad,
    kHistogramDeltaTime,
    kHistogramDeltaSigmaY2,
    kHistogramDeltaSigmaZ2,
    kHistogramDeltaCharge,
    kHistogramDeltaQMax,
    kHistogramOutOfRange,
    kNumberOfHistograms
  };
  enum {
    kHistogramQMaxSector,
    kHistogramSigmaY2Sector,
    kHistogramSigmaZ2Sector,
    kHistogramXYA,
    kHistogramXYC,
    kNumberOfHistograms2D
  };
  enum {
    kHistogramPadrowPadSector,
    kNumberOfHistograms3D
  };

  struct AliHistogramDefinition {
    int fId; //!
    const char* fName; //!
    const char* fTitle; //!
    int fBins; //!
    float fLowerBound; //!
    float fUpperBound; //!
    const char* fDrawOptions; //!
  };
  struct AliHistogramDefinition2D {
    int fId; //!
    const char* fName; //!
    const char* fTitle; //!
    int fBinsX; //!
    float fLowerBoundX; //!
    float fUpperBoundX; //!
    int fBinsY; //!
    float fLowerBoundY; //!
    float fUpperBoundY; //!
    const char* fDrawOptions; //!
  };
  struct AliHistogramDefinition3D {
    int fId; //!
    const char* fName; //!
    const char* fTitle; //!
    int fBinsX; //!
    float fLowerBoundX; //!
    float fUpperBoundX; //!
    int fBinsY; //!
    float fLowerBoundY; //!
    float fUpperBoundY; //!
    int fBinsZ; //!
    float fLowerBoundZ; //!
    float fUpperBoundZ; //!
    const char* fDrawOptions; //!
  };

  /**
   * @class AliDataContainer
   * Cluster read interface for monitoring.
   * The class implements the interface to be used in the decoding
   * of compressed TPC data.
   */
  class AliDataContainer : public AliHLTLogging {
  public:
    AliDataContainer();
    virtual ~AliDataContainer();

    struct AliClusterIdBlock {
      AliClusterIdBlock() : fIds(NULL), fSize(0) {}
      AliHLTUInt32_t* fIds; //!
      AliHLTUInt32_t  fSize; //!
    };

    class iterator {
    public:
      iterator() : fClusterNo(-1), fData(NULL), fClusterId(kAliHLTVoidDataSpec), fSlice(-1), fPartition(-1) {}
      iterator(AliDataContainer* pData) : fClusterNo(-1), fData(pData), fClusterId(fData?fData->GetClusterId(fClusterNo):kAliHLTVoidDataSpec), fSlice(-1), fPartition(-1) {}
      iterator(const iterator& other) : fClusterNo(other.fClusterNo), fData(other.fData), fClusterId(other.fClusterId), fSlice(other.fSlice), fPartition(other.fPartition) {}
      iterator& operator=(const iterator& other) {
	if (this==&other) return *this;
	fClusterNo=other.fClusterNo; fData=other.fData; fClusterId=other.fClusterId; fSlice=other.fSlice; fPartition=other.fPartition; return *this;
      }
      ~iterator() {}

      void SetPadRow(int row)             {if (fData) fData->FillPadRow(row, fSlice, fClusterId);}
      void SetPad(float pad) 	          {if (fData) fData->FillPad(pad, fClusterId);}
      void SetTime(float time) 	          {if (fData) fData->FillTime(time, fClusterId);}
      void SetSigmaY2(float sigmaY2)      {if (fData) fData->FillSigmaY2(sigmaY2, fClusterId, fPartition);}
      void SetSigmaZ2(float sigmaZ2)      {if (fData) fData->FillSigmaZ2(sigmaZ2, fClusterId);}
      void SetCharge(unsigned charge)     {if (fData) fData->FillCharge(charge, fClusterId);}
      void SetFlags(unsigned short flags)   {if (fData) fData->FillFlags(flags, fClusterId);}
      void SetQMax(unsigned qmax)         {if (fData) {fData->FillQMax(qmax, fClusterId);fData->Fill(fSlice, fPartition, fClusterId);}}
      iterator& operator=(const AliHLTTPCRawCluster& cluster) {if (fData) {
	  fData->FillPadRow(cluster.GetPadRow(), fSlice, fClusterId);
	  fData->FillPad(cluster.GetPad(), fClusterId);
	  fData->FillTime(cluster.GetTime(), fClusterId);
	  fData->FillSigmaY2(cluster.GetSigmaPad2(), fClusterId, fPartition);
	  fData->FillSigmaZ2(cluster.GetSigmaTime2(), fClusterId);
	  fData->FillCharge(cluster.GetCharge(), fClusterId);
	  fData->FillQMax(cluster.GetQMax(), fClusterId);
	  fData->FillFlags(cluster.GetFlags(), fClusterId);
	  fData->Fill(fSlice, fPartition, fClusterId);
	} return *this;}
      void SetMC(const AliHLTTPCClusterMCLabel* /*pMC*/) {/* nop */}

      // switch to next cluster
      iterator& Next(int slice, int partition) {
	fSlice=slice; fPartition=partition; return operator++();
      }
      // prefix operators
      iterator& operator++() {fClusterNo++; fClusterId=fData?fData->GetClusterId(fClusterNo):kAliHLTVoidDataSpec;return *this;}
      iterator& operator--() {fClusterNo--; fClusterId=fData?fData->GetClusterId(fClusterNo):kAliHLTVoidDataSpec;return *this;}
      // postfix operators
      iterator operator++(int) {iterator i(*this); fClusterNo++; return i;}
      iterator operator--(int) {iterator i(*this); fClusterNo--; return i;}

      bool operator==(const iterator other) const {return fData==other.fData;}
      bool operator!=(const iterator other) const {return fData!=other.fData;}

    private:
      int fClusterNo; //! cluster no in the current block
      AliDataContainer* fData; //! pointer to actual data
      AliHLTUInt32_t fClusterId; //! id of the cluster, from optional cluster id blocks
      int fSlice;     //! current slice
      int fPartition; //! current partition
    };

    /// legacy, to be removed later
    iterator& BeginRemainingClusterBlock(int count, AliHLTUInt32_t specification) {
      return BeginPartitionClusterBlock(count, specification);
    }
    /// iterator of partition clusters block of specification
    iterator& BeginPartitionClusterBlock(int count, AliHLTUInt32_t specification);
    /// iterator of track model clusters
    iterator& BeginTrackModelClusterBlock(int count);

    /// add raw data bloack
    int AddRawData(const AliHLTComponentBlockData* pDesc);
    /// add cluster id block for remaining or track model clusters
    int AddClusterIds(const AliHLTComponentBlockData* pDesc);
    /// get the cluster id from the current cluster id block (optional)
    AliHLTUInt32_t GetClusterId(int clusterNo) const;
    /// get the cluster id of the nearest original cluster
    AliHLTUInt32_t FindNearestCluster(int slice, int partition, const AliHLTTPCRawCluster& cluster) const;

    bool CheckClusterID(AliHLTUInt32_t clusterID) const {
      if (fRawData) return fRawData->Check(clusterID);
      return false;
    }
    float GetClusterX(AliHLTUInt32_t clusterID) const {
      if (fRawData) return fRawData->GetX(clusterID);
      return false;
    }
    float GetClusterXWidth(AliHLTUInt32_t clusterID) const {
      if (fRawData) return fRawData->GetXWidth(clusterID);
      return false;
    }
    float GetClusterY(AliHLTUInt32_t clusterID) const {
      if (fRawData) return fRawData->GetY(clusterID);
      return false;
    }
    float GetClusterYWidth(AliHLTUInt32_t clusterID) const {
      if (fRawData) return fRawData->GetYWidth(clusterID);
      return false;
    }
    float GetClusterZ(AliHLTUInt32_t clusterID) const {
      if (fRawData) return fRawData->GetZ(clusterID);
      return false;
    }
    float GetClusterZWidth(AliHLTUInt32_t clusterID) const {
      if (fRawData) return fRawData->GetZWidth(clusterID);
      return false;
    }
    float GetClusterCharge(AliHLTUInt32_t clusterID) const {
      if (fRawData) return fRawData->GetCharge(clusterID);
      return false;
    }
    float GetClusterQMax(AliHLTUInt32_t clusterID) const {
      if (fRawData) return fRawData->GetMaxSignal(clusterID);
      return false;
    }
    const AliHLTSpacePointContainer::AliHLTSpacePointPropertyGrid* GetClusterSpacePointPropertyGrid(AliHLTUInt32_t mask) const {
      if (fRawData) return fRawData->GetSpacePointPropertyGrid(mask);
      return NULL;
    }

    /// internal cleanup
    virtual void  Clear(Option_t * option="");
    /// get histogram object
    virtual TObject* FindObject(const char *name) const;
    
  protected:
    void FillPadRow(int row, int slice, AliHLTUInt32_t clusterId);
    void FillPad(float pad, AliHLTUInt32_t clusterId);
    void FillTime(float time, AliHLTUInt32_t clusterId);
    void FillSigmaY2(float sigmaY2, AliHLTUInt32_t clusterId, int partition);
    void FillSigmaZ2(float sigmaZ2, AliHLTUInt32_t clusterId);
    void FillCharge(unsigned charge, AliHLTUInt32_t clusterId);
    void FillQMax(unsigned qmax, AliHLTUInt32_t clusterId);
    void FillFlags(unsigned short flags, AliHLTUInt32_t clusterId);
    void Fill(int slice, int partition, AliHLTUInt32_t clusterId);

  private:
    AliDataContainer(const AliDataContainer&);
    AliDataContainer& operator=(const AliDataContainer&);

    TObjArray* fHistograms;     //! array of histograms
    TObjArray* fHistograms2D;     //! array of histograms
    TObjArray* fHistograms3D;     //! array of histograms
    vector<TH1*> fHistogramPointers; //! pointers to histograms
    vector<TH2*> fHistogram2DPointers; //! pointers to histograms
    vector<TH3*> fHistogram3DPointers; //! pointers to histograms
    vector<AliClusterIdBlock> fRemainingClusterIds; //! clusters ids for remaining cluster ids
    AliClusterIdBlock fTrackModelClusterIds; //! cluster ids for track model clusters
    AliClusterIdBlock* fCurrentClusterIds; //! id block currently active in the iteration
    AliHLTSpacePointContainer* fRawData; //! raw data container
    AliHLTTPCRawCluster fCurrentCluster; //! current cluster
    int fSector; //! sector
    iterator fBegin; //!
    int fMaxSigmaY2Scaled; //! maximum scaled sigmaY2 value determined by the available bit length
    int fMaxSigmaZ2Scaled; //! maximum scaled sigmaZ2 value determined by the available bit length
  };

protected:
  /// inherited from AliHLTProcessor: data processing
  int DoEvent( const AliHLTComponentEventData& evtData, 
	       const AliHLTComponentBlockData* blocks, 
	       AliHLTComponentTriggerData& trigData,
	       AliHLTUInt8_t* outputPtr, 
	       AliHLTUInt32_t& size,
	       AliHLTComponentBlockDataList& outputBlocks );
  using AliHLTProcessor::DoEvent;

  /// inherited from AliHLTComponent: component initialisation and argument scan.
  int DoInit( int argc, const char** argv );

  /// inherited from AliHLTComponent: component cleanup
  int DoDeinit();

  /// inherited from AliHLTComponent: argument scan
  int ScanConfigurationArgument(int argc, const char** argv);

  /// publish to output
  int Publish(int mode);

  AliHLTComponentBenchmark* GetBenchmarkInstance() const {return fpBenchmark;}
    
private:
  AliHLTTPCDataCompressionMonitorComponent(const AliHLTTPCDataCompressionMonitorComponent&);
  AliHLTTPCDataCompressionMonitorComponent& operator=(const AliHLTTPCDataCompressionMonitorComponent&);

  /// benchmark instance
  AliHLTComponentBenchmark* fpBenchmark; //! benchmark instance
  /// cluster decoder instance
  AliHLTTPCDataCompressionDecoder* fpDecoder; //! cluster decoder instance
  /// data decoder for HW clusters
  AliHLTTPCHWCFData* fpHWClusterDecoder; //! data decoder for HW clusters

  TH2* fHistoHWCFDataSize;         //! hwcf data size vs. event size
  TH2* fHistoHWCFReductionFactor;  //! reduction factor vs. event size
  TH2* fHistoTotalReductionFactor;  //! reduction factor vs. event size
  TH2* fHistoNofClusters; //! number of clusters vs. event size
  TH2* fHistoNofClustersReductionFactor;  //! reduction factor wrp to hwcf vs. number of clusters
  TString fHistogramFile; //! file to save histogram
  TString fHuffmanTableFile; //! configuration file to load Huffman table from
  AliDataContainer* fMonitoringContainer; //! cluster read interface for monitoring

  /// verbosity
  int fVerbosity;  //! verbosity for debug printout
  unsigned fFlags; //! flags to indicate various conditions
  int fPublishingMode; //! publishing mode

  static const AliHistogramDefinition fgkHistogramDefinitions[]; //! histogram definitions
  static const AliHistogramDefinition2D fgkHistogramDefinitions2D[]; //! histogram definitions
  static const AliHistogramDefinition3D fgkHistogramDefinitions3D[]; //! histogram definitions

  ClassDef(AliHLTTPCDataCompressionMonitorComponent, 0)
};

#endif //ALIHLTTPCDATACOMPRESSIONMONITORCOMPONENT_H
