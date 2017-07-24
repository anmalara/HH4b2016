{
  std::vector<double> masses_1={260, 270, 300};
  std::vector<double> masses_2={300, 350, 400, 450, 500, 550, 600};
  //std::vector<double> masses_1={260, 265, 270, 275, 280, 285, 290, 295, 300, 310, 320};
  //std::vector<double> masses_2={290, 295, 300, 310, 320, 330, 340, 350, 360, 370, 380, 390, 400, 410, 420, 430, 440, 450, 460, 470, 480, 490, 500, 510, 520, 530, 540, 550, 560, 570, 580, 590, 600};
  gROOT->LoadMacro("DrawLimitPlot.cc++");
  DrawLimitPlot(masses_1, 10, 1e5, "gaus_exp_250_330", 0);
  DrawLimitPlot(masses_2, 10, 1e5, "novo_285_625", 0);
}
