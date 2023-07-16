{
	//TGraph *gr1 = new TGraph("times_lf.txt", "%lg,%*lg,%lg");
    TCanvas *c1 = new TCanvas();
    c1->SetLogy();
    c1->SetLogx();
    c1->SetGridx();
    c1->SetGridy();
    TGraph *lf, *rk1, * rk2;

    bool delE = false;

    if(delE){
        lf = new TGraph("data/lf_staticE_test_results.txt", "%lg %lg %*lg %*lg %*lg");   
    }
    else{
        lf = new TGraphErrors("data/lf_staticE_test_results.txt", "%lg %*lg %*lg %lg %lg");   
    }
    lf->SetMarkerStyle(23);
    lf->SetMarkerColor(kBlue);
    lf->SetTitle("Leap-frog");
/*
    if(delE){
        rk1 = new TGraph("rk1_test_results.txt", "%lg %lg %*lg");   
    }
    else{
        rk1 = new TGraphErrors("rk1_test_results.txt", "%lg %*lg %lg %lg");   
    }
    rk1->SetMarkerStyle(22);
    rk1->SetMarkerColor(kRed);
    rk1->SetTitle("RK4-1");*/

    if(delE){
        rk2 = new TGraph("data/rk_staticE_test_results.txt", "%lg %lg %*lg %*lg %*lg");   
    }
    else{
        rk2 = new TGraphErrors("data/rk_staticE_test_results.txt", "%lg %*lg %*lg %lg %lg");   
    }
    rk2->SetMarkerStyle(21);
    rk2->SetMarkerColor(kRed);
    rk2->SetTitle("RK4-2");
    //plot->Draw("AP");
    TMultiGraph *mg = new TMultiGraph();
    mg->Add(lf, "lp");
    //mg->Add(rk1, "cp");
    mg->Add(rk2, "lp");
    mg->GetXaxis()->SetTitle("dt(ns)");
    mg->GetXaxis()->SetLimits(5e-7,1e-1);
    if(delE){
        mg->GetYaxis()->SetTitle("#DeltaE(MeV)");
        mg->SetMaximum(0.13);
        mg->SetMinimum(1e-6);
    }
    else{
        mg->GetYaxis()->SetTitle("T_{SIM}(s)");
        mg->SetMaximum(450);
        mg->SetMinimum(0.02);
    }

    mg->Draw("AP");
    c1->BuildLegend();
    //mg->Add(gr1,"lp");
    //mg->Add(gr2,"cp");
    //gr1->Draw("a");
}