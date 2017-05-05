Bounding Part Scores for Rapid Detection with Deformable Part Models 

Author: 	  Iasonas Kokkinos, Ecole Centrale Paris/INRIA Saclay
Contact: 	  iasonas.kokkinos@ecp.fr
Project page: http://vision.mas.ecp.fr/Personnel/iasonas/dpms.html 

This project has been supported by Agence Nationale de Recherche (ANR) under Grant ANR-10-JCJC-0205.

------------------------
Introduction 
------------------------
This code uses lookup-based bounds in Dual-Tree Branch-and-Bound (DTBB) and Cascaded detection with Deformable Part Models (C-DPM). The reference
explaining the theory behind the code is [1]; the references for DTBB are [2,2a] and the reference for C-DPM is [4]. 
The code is a c++/matlab hybrid. 

------------------------
Software and licences
------------------------
This release builds and improves upon the software of [1], which in turn builds on the system distributed in [5,6] under the GNU GPL. 
We include a slightly adapted version of [5,6], allowing us to extract timing information, and also to compile it in Windows.  
We use a single-threaded variant of the fconvsse/fconv_varying_dim routines (found in [6]), to allow for a fair comparison between different implementations.
We also include parts of source code from the ESS implementation [7] and the Dual Tree implementation in [9], released under the Apache and the GNU GPL licences respectively. 
Our code is distributed under the GNU-gpl v2. 
------------------------
Usage
------------------------
Unzip the file and move into the formed directory.
The distribution comes with precomputed mex files for 64-bit windows, linux and mac systems. 
If you have a different architecture, run make_dtbb.m

Add all paths below dtbb, and on the command prompt run:
demo_all.m

Please see README_dtbb.txt for additional information on DTBB

------------------------
How to cite
------------------------
We request that you cite [1] when using our code for your academic work, and [2,2a] if you also use DTBB. If you use some of the extensions in [2a], please cite appropriately. 

------------------------
References
------------------------
[1]  Iasonas Kokkinos. Bounding Part Scores for Rapid Detection with Deformable Part Models, 2nd Parts and Attributes Workshop, ECCV 2012.
[2]  Iasonas Kokkinos. Rapid Deformable Object Detection using Dual Tree Branch and Bound. In Neural Information Processing Systems (NIPS) 2011.
[2a] Iasonas Kokkinos. Rapid Deformable Object Detection using Bounding-based Techniques. INRIA Research Report 7940, 2012.
[3] P. Felzenszwalb, R. Girshick, D. McAllester, D. Ramanan. Object Detection with Discriminatively Trained Part Based Models.
IEEE Transactions on Pattern Analysis and Machine Intelligence, Vol. 32, No. 9, Sep. 2010.
[4] P. Felzenszwalb, R. Girshick, D. McAllester. Cascade Object Detection with Deformable Part Models. Proceedings of the IEEE CVPR 2010. 
[5] P. Felzenszwalb, R. Girshick, D. McAllester. Discriminatively Trained Deformable Part Models, Release 4. http://www.cs.brown.edu/~pff/latent-release4/
[6] P. Felzenszwalb, R. Girshick, D. McAllester. Discriminatively Trained Deformable Part Models, Release 5. http://www.cs.brown.edu/~pff/latent-release5/
[7] A. Gray and A. Moore, Very Fast Multivariate Kernel Density Estimation via Computational Geometry., in Proceedings Joint Stat. Meeting 2003.
[8] C. Lampert, M B. Blaschko and T. Hofmann. Efficient Subwindow Search: A Branch and Bound Framework for Object Localization. 
IEEE Transactions on Pattern Analysis and Machine Intelligence, Vol. 31, No. 12, Dec. 2009.
[9] C. H. Lampert. An efficient divide-and-conquer cascade for nonlinear object detection. In CVPR, 2010.
[10] A. Ihler, E. Sudderth, W. Freeman, A. Willsky. Efficient Multiscale Sampling from Products of Gaussian Mixtures. In Neural Information Processing Systems (NIPS) 2003.
