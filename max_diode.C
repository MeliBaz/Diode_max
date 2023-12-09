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
   //Plotting a function which mimics the distribution of laser intensity on a diode
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
  //Finding the maximum value to compare with the obtained value 
   double max=dth->GetMaximum();
   double max_bin=dth->GetMaximumBin();
   int bin_x,bin_z,bin_y;
   dth->GetBinXYZ(max_bin, bin_x,bin_y, bin_z); 
   double max_x=dth->GetXaxis()->GetBinCenter(bin_x);
   double max_y=dth->GetYaxis()->GetBinCenter(bin_y);
//Applying Minuit2 library
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
