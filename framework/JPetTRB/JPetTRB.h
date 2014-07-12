// JPetTRB.h - TRB
#ifndef _JPETTRB_H_
#define _JPETTRB_H_

#include "TNamed.h"
#include <TRef.h>
#include "../JPetUser/JPetUser.h"
#include "../JPetTOMB/JPetTOMB.h"


class JPetTRB: public TNamed
{
protected:
  struct JPetTRBChannel
  {
    JPetTRBChannel(void);
    JPetTRBChannel(int p_id, 
		   bool p_isActive, 
		   std::string p_status, 
		   int p_portNumber, 
		   std::string p_description);
    
    int fId;
    bool fIsActive;
    std::string fStatus;
    int fPortNumber;
    std::string fDescription;
  };
  
  struct JPetTRBInput : public JPetTRBChannel
  {
    JPetTRBInput(void);
    JPetTRBInput(int p_id, 
		 bool p_isActive, 
		 std::string p_status, 
		 int p_portNumber, 
		 std::string p_description, 
		 int p_TRBId);
    
    int fTRBId;
  };

  struct JPetTRBOutput : public JPetTRBChannel
  {
    JPetTRBOutput(void);
    JPetTRBOutput(int p_id, 
		  bool p_isActive, 
		  std::string p_status, 
		  int p_portNumber, 
		  std::string p_description, 
		  int p_TRBId,
		  int p_TRBInputId);
    
    int fTRBId;
    int fTRBInputId;
  };
  
public:
  JPetTRB(void);
  JPetTRB(int p_id, bool p_isActive, std::string p_status, int p_maxch, std::string p_name, std::string p_description, int p_version, const JPetUser &p_user);
  ~JPetTRB(void);
  
  int getId() const { return fId; }
  bool getIsActive() const { return fIsActive; }
  std::string getStatus() const { return fStatus; }
  int getMaxch() const { return fMaxch; }
  std::string getName() const { return fName; }
  std::string getDescription() const { return fDescription; }
  int getVersion() const { return fVersion; }
  JPetUser getUser() const { return fUser; }
  
  TRef getTRefTOMB() const { return fTRefTOMB; }
  void setTRefTOMB(JPetTOMB &p_TOMB)
  {
    fTRefTOMB = &p_TOMB;
  }
  
  std::vector<JPetTRB::JPetTRBInput> getInputs() const { return fInputs; }
  void setInputs(std::vector<JPetTRBInput> &p_Inputs)
  {
    fInputs = p_Inputs;
  }
  void addInput(JPetTRBInput &p_Input)
  {
    fInputs.push_back(p_Input);
  }
  std::size_t getInputsSize() const
  {
    return fInputs.size();
  }
  
  std::vector<JPetTRB::JPetTRBOutput> getOutputs() const { return fOutputs; }
  void setOutputs(std::vector<JPetTRBOutput> &p_Outputs)
  {
    fOutputs = p_Outputs;
  }
  void addOutput(JPetTRBOutput &p_Output)
  {
    fOutputs.push_back(p_Output);
  }
  std::size_t getOutputsSize() const
  {
    return fOutputs.size();
  }
  
  friend class JPetParamManager;
  
protected:
  int fId;
  bool fIsActive;
  std::string fStatus;
  int fMaxch;
  std::string fName;
  std::string fDescription;
  int fVersion;
  JPetUser fUser;
  
  TRef fTRefTOMB;
  std::vector<JPetTRBInput> fInputs;
  std::vector<JPetTRBOutput> fOutputs;
  
  void clearTRefTOMB()
  {
    fTRefTOMB = NULL;
  }
  
  void clearInputs()
  {
    fInputs.clear();
  }
  
  void clearOutputs()
  {
    fOutputs.clear();
  }
  
  ClassDef(JPetTRB, 1);
};

#endif	// _JPETTRB_H_
