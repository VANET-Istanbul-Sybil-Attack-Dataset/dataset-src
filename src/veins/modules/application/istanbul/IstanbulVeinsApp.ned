package org.car2x.veins.modules.application.istanbul;

import org.car2x.veins.modules.application.f2md.JosephVeinsApp;

simple IstanbulVeinsApp extends JosephVeinsApp
{
	@class(IstanbulVeinsApp);
	
	double probReplaySybil = default(1.0);
	double probRandomSybil = default(1.0);
	double probSimilarRandomSybil = default(1.0);
	double probGridSybil = default(1.0);
	
	double probPowerControl = default(0.75);

	// replay, random, similarRandom
	int minSybilNodeCount = default(2);  // min 2
	int maxSybilNodeCount = default(8);
}
