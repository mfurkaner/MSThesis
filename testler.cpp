
int phase_opt(int phase_sweep_range){
    double minrms = 1;
    int opt_phase;
    for(int RFphase = -phase_sweep_range; RFphase <= phase_sweep_range; RFphase++){
      Bunch bunch1(RFphase);
      double t1 = 0;
      bunch1.bunch_gecis_t(t1);
      bunch1.reset_pos();

      if( bunch1.E_rms() < minrms ){
        minrms = bunch1.E_rms();
        opt_phase = RFphase;
      }
    }
    return opt_phase;
}


int phase_opt(const vector<double>& Louts, int phase_sweep_range){
    double minrms = 1;
    int opt_phase;
    for(int RFphase = -phase_sweep_range; RFphase <= phase_sweep_range; RFphase++){
        Bunch bunch1(RFphase);
        double t1 = 0;
        bunch1.bunch_gecis_t(t1);
        bunch1.reset_pos();

        for(int i = 0; i < Louts.size(); i++){
            bunch1.bunch_gecis_d(Louts[i]);
            bunch1.reset_pos();
        }
            
        if( bunch1.E_rms() < minrms ){
            minrms = bunch1.E_rms();
            opt_phase = RFphase;
        }
    }
    return opt_phase;
}



class Electron{
    std::vector<ElectronLog> log;
    uint64_t index;
    double Et;
    vector3d pos;
    vector3d vel;
    double fire_time;
public:
    Electron();
    Electron(double Ein, vector3d position, vector3d direction, double _fire_time = 0);
    ~Electron();

    void setEin(double E_in);
    void print_electron_info();

    void move(double dt);
    void move(const vector3d& acc, double dt);
    void move(const vector3d& acc, const vector3d& jerk, double dt);
    void accelerate(const vector3d& acc, double dt);
    void accelerate(const vector3d& acc, const vector3d& jerk, double dt);
    
    void interactLF(RFField& E, MagneticField& B, double time_interval);
    void interactRK(RFField& E, MagneticField& B, double time_interval);
    void interactRK(RFField& E, MagneticField& B, const double time, double time_interval);
    void interactRK_ActorE(const RFField& E, const MagneticField& B, double time_interval);

    vector3d interactE_RK(const RFField& E, double time_interval);
    vector3d interactB_RK(const MagneticField& B, double time_interval);

    void saveInfo(double t);
    void setLogSize(size_t size);
    void loge(DataStorage& path);
    const vector<ElectronLog>& getLog();

    double vel();
    double beta();
    double beta2();
    double gamma();
    double gamma_inv();
};

class Bunch{
private:
    int e_count = 0;
    int index_fastest = 0;
    double max_energy = 0;
    double entry_time = 0;
    double E_in = 0.04;

    vector<Bunch> subBunchs;
    vector<shared_ptr<Electron>> e;

public:

    Bunch(unsigned int num_of_electrons, double Ein, vector3d gunpos, vector3d gundir, double gun_ns);
    Bunch();

    void saveInfo(double time);

    Electron& getFastest();

    void AddElectron(double Ein, const vector3d& gunpos, const vector3d& gundir, double fire_time);

    shared_ptr<Electron> AddElectronGiveAddress(double Ein, const vector3d& gunpos, const vector3d& gundir, double fire_time);

    void setEntryTime(double entry_time);
    void setEin(double E_in);
    void setNSLen(double len);

    uint32_t get_e_count();
    double getEin();
    double E_ave();
    double E_rms();

    void interact(RFField& E, MagneticField& B, const double time, double time_interval);

    void divide(unsigned int num);
    void concat();
    Bunch& subBunch(unsigned int index);
    vector<Bunch*> subBunchPtr();

    void reset();

    void print_bunch_info();
    void print_summary();

    void logPaths(vector<DataStorage>& pathStorage, std::string header);
};


class Gun{
    double Ein;
    double sEin;
    double gun_period;
    double gun_active_time;
    double ns_between_each_electron_fire;

    vector3d gunpos;
    vector3d gundir = vector3d(1,0,0);

    uint64_t bunch_count = 0;
    uint64_t e_per_bunch = 1;

    uint32_t _fired_bunch = 0;
    uint32_t _fired_e_in_current_bunch = 0;
    bool _firing = false;

    bool _mt_enabled = false;
    uint32_t _child_thread_count = 0;

    vector<Bunch> bunchs;

    mutex _gun_mutex; 
    vector<shared_ptr<vector<shared_ptr<Electron>>>> thread_bunchs;

    void setNSLen(double len);
public:
    Gun();
    Gun(double Ein, double gun_active_time, double pulse_interval, vector3d gunpos);
    ~Gun();
    void interact(RFField& E, MagneticField& B, const double time, double time_interval);

    void fireIfActive(double time);
    void fireAllWithFireTimesMT();
    
    void addBunch(unsigned int num_of_electrons, double Ein);

    void setGunActiveTime(double gt);
    double getGunActiveTime(){return gun_active_time;}
    void setGunInterval(double guninterval);
    void setGunPos(vector3d gun_pos);
    void setGunDir(vector3d gun_dir);
    void setEin(double Ein) ;
    void setEinStd(double EinStd) ;
    void setNumberOfElectrons(uint64_t e_num);
    void getElectronsPerBunch(){return e_per_bunch;}
    void setNumberOfBunchs(uint64_t b_num);
    void enableMT(uint32_t thread_count);

    void saveInfo(double time);
    void logPaths(vector<vector<DataStorage> >& pathsStorage, std::string pathsPath, std::string header);
};

class Simulator{        // E in MV/m,   En in MeV,   B in T,    t in ns
protected:
    Gun gun;
    double Emax; double freq = 107.5; double phase_lag = 0; 
    DataStorage EfieldStorage;
    DataStorage BfieldStorage;
    vector<vector<DataStorage>> pathsStorage;
    std::string configPath;
    std::string pathsPath;

    std::string EfieldLogHeader;
    std::string BfieldLogHeader;
    std::string eLogHeader;

    double simulation_time = 0;
    double dummy_time = 0;
    double start_time = 0;
    double end_time = 45;   

    double time_interval = 0.0001;
    uint64_t STEPS_TAKEN = 0;
    double GUN_ACTIVE_TIME = 1;         
    double GUN_PERIOD = 9.3;
    uint64_t NUM_OF_ELECTRONS = 1;
    uint64_t NUM_OF_BUNCHS = 1;
    double Ein = 0.04;
    double EinStd = 0;

    vector3d gunPosition;
    vector3d gunDirection = vector3d(1,0,0);

    shared_ptr<mutex> ui_mutex;
    UIHandler ui_handler;

    shared_ptr<mutex> state_mutex;
    uint8_t state = 0x0;

    MultiThreadEngine MTEngine;
    unsigned int MAX_THREAD_COUNT = 1;
    bool MULTI_THREAD = false;
    bool DEBUG = false;

public:
    Simulator();
    ~Simulator(){}

    void enableMultiThreading(unsigned int thread_count);

    void setdT(double dT);
    void setEin(double E_in);
    void setEinStd(double Ein_Std);
    void setNumberofElectrons(uint64_t num_of_electrons);
    void setNumberofBunchs(uint64_t num_of_bunchs);
    void setStartTime(double starttime);
    void setEndTime(double end_time);
    void setGunActiveTime(double gun_ns);
    void setGunPeriod(double pi);
    void setGunPosition(vector3d pos);
    void setGunDirection(vector3d dir);

    void setRFPath(std::string path);
    void setBPath(std::string path);
    void setPathsPath(std::string path);
    void setConfigPath(std::string path);

    virtual void run();
    void saveElectronsInfo(double time);
    void openLogs();
    void closeLogs();

    const unsigned int log_interval();

    virtual void getConfig(Configuration& config) = 0;
    virtual void logEfield(double time, bool) = 0;
    virtual void logBfield() = 0;
    void logPaths();
    double* getTimePtr();
    shared_ptr<mutex> getUIMutex();
};

class RhodotronSimulator : public Simulator{
private:
    double R1;
    double R2;
    CoaxialRFField E_field;
    MagneticField B_field;

    void _runMT();
    void _runST();
public:
    RhodotronSimulator(double phase_lag);
    RhodotronSimulator(Configuration& config);

    void StartUIHandler();
    void StopUIHandler();
    void DeclareService(std::string pipe_name);

    void setFreq(double frequency);
    void setPhaseLag(double phase_lag);
    void setEmax(double E_max);
    void setR1(double r1);
    void setR2(double r2);
    void addMagnet(double B, double r, vector3d position);
    void addMagnet(Magnet m);
    
    void updateSimulation();
    void getConfig(Configuration& config);
    void logEfield(double time, bool end);
    void logBfield();

    void run();
    void notifyUI(double time);

    void setState(uint8_t state_);
};


class RFField{
protected:
    double E;                       // MV/m
    double E_max = 0;               // MV/m
    double frequency = 107.5;       // MHz
    double phase_lag = 0;           // degree
public:
    RFField();
    RFField(double phase_lag);
    virtual ~RFField() {}

    virtual vector3d getField(vector3d position);              
    virtual double getField(double R);
    virtual int log(DataStorage& rf, double time, bool end = false);
    vector3d actOn(Electron& e);    
    vector3d actOnAndGetRungeKuttaCoef(Electron& e, double dt);                  
    
    double getE() {return E;}
    void setEmax(double E_max) {this->E_max = E_max; update(0);}
    void setFreq(double freq){ frequency = freq;}
    void setPhaseLag(double phaselag){ phase_lag = phaselag; update(0);}
    virtual void update(double time);
};

class CoaxialRFField : public RFField{
private:
    double r1 = 0.188;                         // m
    double r2 = 0.753;                         // m
    double E_max_pos = r1;                     // m
    std::vector<CoaxialRFField*> _childs;
    double E_radial(double R) const;
public:
    CoaxialRFField();
    CoaxialRFField(double phase_lag);
    ~CoaxialRFField() override;
    void setR1(double r1);
    void setR2(double r2);
    void setEmaxPos(double Emaxpos);   

    vector3d getField(vector3d position) const override;
    double getField(double R) const override;
    vector3d actOn(Electron& e);
    vector3d actOnAndGetRungeKuttaCoef(Electron& e, double dt);    
    void update(double time) override; 

    int log(DataStorage& rf, double time, bool end = false) override;

    std::shared_ptr<CoaxialRFField> Copy();
    void split(uint32_t amount_of_child);
    CoaxialRFField* child(uint32_t index);
};