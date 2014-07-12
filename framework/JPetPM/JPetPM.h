// JPetPM.h - Photomultiplier
#ifndef _JPET_PM_
#define _JPET_PM_

#include "TNamed.h"
#include <utility>
#include <TRef.h>
#include <vector>
#include "../JPetUser/JPetUser.h"
#include "../JPetKB/JPetKB.h"
//#include "../JPetParamManager/JPetParamManager.h"


class JPetPM: public TNamed
{
protected:
  enum Side {kLeft, kRight};

  struct PMModel
  {
    int fId;
    std::string fName;
    
    PMModel(int p_id, std::string p_name) : fId(p_id), fName(p_name)
    {}
  };
  
  struct PMCalibration
  {
    int fId;
    std::string fName;
    float fOptHV;
    float fC2e_1;
    float fC2e_2;
    float fGainAlpha;
    float fGainBeta;
    
    PMCalibration(int p_id, 
		  std::string p_name, 
		  float p_optHV, 
		  float p_c2e_1, 
		  float p_c2e_2, 
		  float p_gainAlpha, 
		  float p_gainBeta) : 
				     fId(p_id), 
				     fName(p_name), 
				     fOptHV(p_optHV), 
				     fC2e_1(p_c2e_1), 
				     fC2e_2(p_c2e_2), 
				     fGainAlpha(p_gainAlpha), 
				     fGainBeta(p_gainBeta)
    {}
  };

public:
  JPetPM(void);
  JPetPM(JPetPM::Side p_side,
	 int p_id,
	 bool p_isActive, 
	 std::string p_status, 
	 std::string p_name, 
	 double p_maxHV, 
	 std::string p_description, 
	 std::string p_producer,
	 std::string p_boughtDate,
	 std::string p_serialNumber,
	 bool p_takeSpositiveVoltage,
	 int p_modelId,
	 std::string p_modelName,
	 int p_calibrationId,
	 std::string p_calibrationName,
	 float p_calibrationOptHV,
	 float p_calibrationC2e_1,
	 float p_calibrationC2e_2,
	 float p_calibrationGainAlpha,
	 float p_calibrationGainBeta,
	 JPetUser fUser);
  ~JPetPM(void);
  
  Side getSide() const { return fSide; }
  int getId() const { return fId; }
  bool getIsActive() const { return fIsActive; }
  std::string getStatus() const { return fStatus; }
  std::string getName() const { return fName; }
  double getMaxHV() const { return fMaxHV; }
  std::string getDescription() const { return fDescription; }
  std::string getProducer() const { return fProducer; }
  std::string getBoughtDate() const { return fBoughtDate; }
  std::string getSerialNumber() const { return fSerialNumber; }
  bool getTakeSpositiveVoltage() const { return fTakeSpositiveVoltage; }
  PMModel getPMModel() const { return fPMModel; }
  PMCalibration getCalibration() const { return fPMCalibration; }
  JPetUser getUser(void) const { return fUser; }
  
  std::vector<TRef> getTRefKBs() const { return fTRefKBs; }
  void setTRefKBs(std::vector<TRef> &p_TRefKBs)
  {
    fTRefKBs = p_TRefKBs;
  }
  void addTRefKB(JPetKB &p_KB)
  {
    fTRefKBs.push_back(&p_KB);
  }
  
  friend class JPetParamManager;
  
protected:
  Side fSide;
  int fId;
  bool fIsActive;
  std::string fStatus;
  std::string fName;
  double fMaxHV;
  std::string fDescription;
  std::string fProducer;
  std::string fBoughtDate;
  std::string fSerialNumber;
  bool fTakeSpositiveVoltage;
  PMModel fPMModel;
  PMCalibration fPMCalibration;
  JPetUser fUser;
  
  std::vector<TRef> fTRefKBs;
  
  void clearTRefKBs()
  {
    fTRefKBs.clear();
  }

  ClassDef(JPetPM, 1);
};

#endif	// _JPET_PM_
