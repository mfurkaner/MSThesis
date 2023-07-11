
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