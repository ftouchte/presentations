#include "TGraph.h"
#include "TCanvas.h"
#include "TMultiGraph.h"
#include "TLegend.h"
#include "TLine.h"
#include "TPolyLine.h"
#include "TColor.h"
#include <iostream>

double compute_transparence(double max_opacity, int nsteps) {
    return 1-pow(1-max_opacity, 1.0/nsteps);
}

// Astuce : dessiner le polygone N fois en le « réduisant » vers son centroïde, avec une couleur de plus en plus foncée
void gradient_polygon(TGraph* g, int color, int nsteps, double max_opacity)
{
    int n = g->GetN() - 1;               // dernier point = premier (fermeture), on l'exclut
    double cx = g->GetX()[0];
    double cy = g->GetY()[0];

    for (int s = 0; s < nsteps; s++) {
        double t = double(s) / (nsteps - 1);        // 0 -> 1
        double scale = 1-t;                     // du grand au petit
        std::vector<double> x(n+1), y(n+1);
        for (int i = 0; i < n; i++) {
            x[i] = cx + scale*(g->GetX()[i] - cx);
            y[i] = cy + scale*(g->GetY()[i] - cy);
        }
        x[n] = x[0]; y[n] = y[0];
        auto p = new TPolyLine(n+1, x.data(), y.data());
        p->SetFillStyle(1001);
        //p->SetFillColorAlpha(kRed, 0.08);
        p->SetFillColorAlpha(color, compute_transparence(max_opacity, nsteps));
        p->SetLineWidth(0);
        p->Draw("f same");
    }
}




void phase_space_comparison() {

    TGraph* gr_alert = new TGraph();
    gr_alert->AddPoint(0.06,1);
    gr_alert->AddPoint(0.25,1.2);
    gr_alert->AddPoint(0.35,1.75);
    gr_alert->AddPoint(0.28,4.7);
    gr_alert->AddPoint(0.06,1);
    gr_alert->SetLineColor(kRed);
    gr_alert->SetFillColor(kRed-10);
    gr_alert->SetFillStyle(1001);

    TGraph* gr_alert_contour = new TGraph();
    gr_alert_contour->AddPoint(0.28,4.7);
    gr_alert_contour->AddPoint(0.06,1);
    gr_alert_contour->AddPoint(0.25,1.2);
    gr_alert_contour->AddPoint(0.35,1.75);
    gr_alert_contour->SetLineColor(kRed);


    TGraph* gr_eg6 = new TGraph();
    gr_eg6->AddPoint(0.1,1);
    gr_eg6->AddPoint(0.15,1);
    gr_eg6->AddPoint(0.36,2);
    gr_eg6->AddPoint(0.39,3.5);
    gr_eg6->AddPoint(0.1,1);
    gr_eg6->SetLineColor(kBlue);
    gr_eg6->SetFillColor(kBlue-10);
    gr_eg6->SetFillStyle(1001);

    TGraph* gr_eg6_contour = new TGraph();
    gr_eg6_contour->AddPoint(0.39,3.5);
    gr_eg6_contour->AddPoint(0.1,1);
    gr_eg6_contour->AddPoint(0.15,1);
    gr_eg6_contour->AddPoint(0.36,2);
    gr_eg6_contour->SetLineColor(kBlue);

    TMultiGraph* mg = new TMultiGraph();
    mg->SetTitle("; xB; Q^{2} (GeV^{2})");
    // mg->Add(gr_alert, "f");
    // mg->Add(gr_eg6, "f");
    mg->Add(gr_alert_contour, "l");
    mg->Add(gr_eg6_contour, "l");

    TLegend* legend = new TLegend(0.1,0.7,0.4,0.9);
    legend->AddEntry(gr_alert, "ALERT",  "lf");
    legend->AddEntry(gr_eg6, "CLAS eg6", "lf");
    legend->Draw("same");
    


    TCanvas* c = new TCanvas("phase_space","phase_space", 800, 600);
    mg->Draw("aplf");
    c->Draw();
    legend->Draw("same");

    gradient_polygon(gr_alert, kRed, 100, 0.7);
    gradient_polygon(gr_eg6, kBlue, 100, 0.7);

    c->Print("phase_space.pdf");
}

// Comments from Claude
// L’opacité cumulée vient de la composition alpha classique : chaque couche posée sur le fond laisse passer une fraction (1 - α) de ce qu’il y a en dessous.

// Principe

// Pour une couche d’opacité α posée sur un fond :

// résultat = α · couleur + (1 − α) · fond

// La part du fond qui « survit » après une couche est donc (1 − α). Après k couches identiques, la part du fond encore visible est (1 − α)^k, et l’opacité cumulée (la part de la couleur) est :

// opacité cumulée = 1 − (1 − α)^k
// Application à votre cas

// Les polygones sont emboîtés : un point situé à une certaine distance de l’ancrage est couvert uniquement par les polygones assez grands pour l’atteindre. Le nombre de couches k dépend donc de la position :

// Au bord extérieur : seul le plus grand polygone le couvre, donc k = 1 et l’opacité vaut 1 − (1 − 0.08)^1 = 0.08.
// Au point d’ancrage : toutes les couches le couvrent, donc k = 30 :
// 1 − (1 − 0.08)^30 = 1 − 0.92^30 ≈ 1 − 0.082 ≈ 0.92

// Avec 0.92^30 : ln(0.92) ≈ −0.0834, multiplié par 30 donne −2.50, et e^(−2.50) ≈ 0.082.

// Opacité à une position intermédiaire

// Si le point est atteint par k des 30 couches, l’opacité vaut 1 − 0.92^k :

// k (couches)	Opacité
// 1	0.08
// 5	0.34
// 10	0.57
// 20	0.81
// 30	0.92
// Choisir α pour viser une opacité au centre

// Si vous voulez une opacité maximale A au centre avec N couches, on inverse la formule :

// α = 1 − (1 − A)^(1/N)

// Par exemple pour A = 0.9 et N = 30 :

// α = 1 − 0.1^(1/30) ≈ 1 − 0.926 ≈ 0.074