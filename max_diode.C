void maxPosition1(double x_scan, double y_scan, double step, TH2D *dth, double &max_value, double &max_position_x, double &max_position_y, int &measurements)
{
    double temp;
    double min_value = 1;
    max_value = 0;
    for (double dx = -x_scan / 2.; dx < x_scan / 2.; dx += step)
    {
        for (double dy = -y_scan / 2.; dy < y_scan / 2.; dy += step)
        {
            measurements++;
            Int_t bin = dth->FindBin(dx, dy);
            double value = dth->GetBinContent(bin);
            //cout<<value<<endl;
            temp = value;

            if (temp < min_value)
                min_value = temp;
            if (temp > max_value)
            {
                max_value = temp;
                max_position_x = dx;
                max_position_y = dy;
            }
        }
    }
}

 TH2D *dth;
 int measurement2=-1;
 TGraph *pathGraph=new TGraph();

double Function(const double *xx )
{
  const double x = xx[0];
  const double y = xx[1];
  Int_t bin = dth->FindBin(x, y);
  double value = dth->GetBinContent(bin);
  double v=2-value;
  measurement2 ++;
  cout<<x<<" "<<y<<" v squared "<<fabs(v*v)<<" Value "<< value<<endl;
  pathGraph->SetPoint(measurement2,x,y);
  return fabs(v*v);
}
void max_diode(){

   TCanvas *c = new TCanvas("c","Graph2D example",0,0,600,400);
   Double_t x, y, z, P = 4.;
   Int_t np = 1200;
   TGraph2D *dt = new TGraph2D();

   TRandom *r = new TRandom();
   for (Int_t N=0; N<np; N++) {
      x = 2*P*(r->Rndm(N))-P;
      y = 2*P*(r->Rndm(N))-P;
      z = (sin(x)/x)*(sin(y)/y)+0.2;
      dt->SetPoint(N,x,y,z);
   }
   gStyle->SetPalette(1);
   dt->SetNpx(100);
   dt->SetNpy(100);
   //dt->Draw("colzp");
   
   
 
   dth= dt->GetHistogram();
   dth->Draw("colzp");
   double max=dth->GetMaximum();
   double max_bin=dth->GetMaximumBin();
   int bin_x,bin_z,bin_y;
   dth->GetBinXYZ(max_bin, bin_x,bin_y, bin_z); 
   double max_x=dth->GetXaxis()->GetBinCenter(bin_x);
   double max_y=dth->GetYaxis()->GetBinCenter(bin_y);
   
   //cout<<"Max x "<<max_x<<" Max y "<<max_y<<" Max "<<max<<endl;
   double x_scan=10;
   double y_scan=10;
   double step=1;
   double max_value, max_position_x, max_position_y;
   int measurements= 0;
   maxPosition1(x_scan,y_scan,step,dth,max_value,max_position_x,max_position_y,measurements);
   cout<<measurements<<" Max_value "<<max_value<<" Maximum position x "<<max_position_x<<" Maximum position y "<< max_position_y<<endl;
   
   ROOT::Math::Minimizer* minimum =
      ROOT::Math::Factory::CreateMinimizer("Minuit2","Simplex");
   
   // set tolerance , etc...
   minimum->SetMaxFunctionCalls(30); // for Minuit/Minuit2
   //minimum->SetMaxIterations(1000);  // for GSL
   minimum->SetTolerance(0.1);
   minimum->SetPrintLevel(1);
 
   // create function wrapper for minimizer
   // a IMultiGenFunction type
   ROOT::Math::Functor f(&Function,2);
   
   // starting point
 
   minimum->SetFunction(f);
 
   // Set the free variables to be minimized !
   minimum->SetVariable(0,"x",2, 0.2);
   minimum->SetVariable(1,"y",2, 0.2);
 
   // do the minimization
   minimum->Minimize();
   
   double v_min=sqrt(minimum->MinValue())-2 ;
   const double *xs = minimum->X();
   std::cout << "Minimum: f(" << xs[0] << "," << xs[1] << "): "
             << minimum->MinValue()  <<" Min value "<<v_min <<" Iterations "<<measurement2<<std::endl;

TH2D *surfacePlot=dth;
TCanvas *c1=new TCanvas();
surfacePlot->Draw("pcolz");
 pathGraph->SetMarkerStyle(20);
 pathGraph->SetMarkerSize(0.5);
 pathGraph->SetMarkerColor(kBlack);
pathGraph->Draw("LP");


}
