void method_4points(){ 
Double_t P = 6.; 
Int_t np = 1200; 
Double_t start_scan = 0; 
Double_t x_scan = 4; 
Double_t y_scan = 3; 
Double_t best_x,best_y, best_value, new_value,new_x,new_y, max_x,max_y;
vector<double>values; 
vector<double>positions_x;
vector<double>positions_y;
Double_t max_value;
int iteration;
//Plotting a function which mimics the distribution of laser intensity on a diode
TCanvas *c = new TCanvas("c", "Graph2D example", 0, 0, 600, 400);
TGraph2D *dt = new TGraph2D();
TRandom *r = new TRandom();
for (Int_t N = 0; N < np; N++) {
    Double_t x = 2 * P * (r->Rndm(N)) - P;
    Double_t y = 2 * P * (r->Rndm(N)) - P;
    Double_t z = (sin(x) / x) * (sin(y) / y) + 0.2;
    dt->SetPoint(N, x, y, z);
}
gStyle->SetPalette(1);
dt->SetNpx(100);
dt->SetNpy(100);
dt->SetTitle(" ;x; y");
//dt->Draw("colzp");
auto dth = dt->GetHistogram();
double max=dth->GetMaximum();
double min=dth->GetMinimum();
dth->Draw("colzp");
TGraph *pathGraph = new TGraph();
pathGraph->SetMarkerStyle(20);
pathGraph->SetMarkerSize(0.5);
pathGraph->SetMarkerColor(kBlack);
    //Selecting the starting point, either using the random function or specifying a particular value.
    double random_x=2.5;
    double random_y=-2;
    //int random_x = start_scan + (x_scan - start_scan) * gRandom->Rndm();
    //int random_y = start_scan + (y_scan - start_scan) * gRandom->Rndm();
    best_x = random_x;
    best_y = random_y;
    cout<<"random_x "<<random_x<<" random_y "<<random_y<<endl;
    //Finding the function value based on the laser position, i.e., based on the x and y coordinates.
    best_value = dth->GetBinContent(dth->FindBin(best_x, best_y));
    
int graphi=-1;
for(iteration=0;iteration<30;iteration++) {
    //Finding four points in the vicinity of the test point.
      for (int i = 0; i < 4; i++) {
         new_x = best_x + (gRandom->Rndm() - 0.5);
         new_y = best_y + (gRandom->Rndm() - 0.5);
         new_value = dth->GetBinContent(dth->FindBin(new_x, new_y));
         cout<<"new_x "<<new_x<<" new_y "<<new_y<<endl;
         values.push_back(new_value);
         positions_x.push_back(new_x);
         positions_y.push_back(new_y);

    }
        
        //Finding the maximum among four points located in the vicinity of the test point
        max_value=values[0];
        max_x=positions_x[0];
        max_y=positions_y[0];
        for(int i=0;i<values.size();i++){
            cout<<values[i]<<" - "<< max_value<<" "<<positions_x[i]<<endl;
             if (values[i] > max_value) {
                 max_value = values[i];  
                 max_x=positions_x[i];
                 max_y=positions_y[i];
        }

        }
        //Assigning a new value to the maximum if the maximum point among the four points is greater than the test point.
        cout<<" "<<max_value<<" max_x "<<max_x<<" max_y "<<max_y<<endl; 
        if (max_value > best_value) {
            best_x = max_x;
            best_y = max_y;
            best_value = max_value;
            pathGraph->SetPoint(++graphi, best_x, best_y);
        }
    
   //Stopping condition for the algorithm 
   if (max_value!=best_value && fabs(max_value - best_value) < 0.001) {
        break;
    }
 
values.clear(); 
positions_x.clear();
positions_y.clear();
   
}

cout<<best_x<<" "<<best_y<<" Max value "<<best_value<<endl;
cout<<"Max "<<max<<" Min "<<min<<endl;
cout<<"Number of iterations "<<iteration<<endl;


TH2D *surfacePlot = dth; 
TCanvas *c1 = new TCanvas("c1","Graph2D ",0,0,600,400);
surfacePlot->Draw("pcolz");
pathGraph->Draw("LP");
}
