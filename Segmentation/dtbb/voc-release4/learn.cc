#define WIN
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//#include "general.h"
#ifndef WIN
#include <sys/time.h>
#else
#define INFINITY (numeric_limits<double>::infinity( ))
#endif

#include <errno.h>
#include <limits>
#include <fstream>
#include <iostream>
using namespace std;

/*
 * Optimize LSVM objective function via gradient descent.
 *
 * We use an adaptive cache mechanism.  After a negative example
 * scores beyond the margin multiple times it is removed from the
 * training set for a fixed number of iterations.
 */

// Data File Format
// EXAMPLE*
// 
// EXAMPLE:
//  long label          ints
//  blocks              int
//  dim                 int
//  DATA{blocks}
//
// DATA:
//  block label         float
//  block data          floats
//
// Internal Binary Format
//  len           int (byte length of EXAMPLE)
//  EXAMPLE       <see above>
//  unique flag   byte

// number of iterations
//#define ITER 10e6

// minimum # of iterations before termination
//#define MIN_ITER 5e6

// convergence threshold
#define DELTA_STOP 0.99995

// number of times in a row the convergence threshold
// must be reached before stopping
#define STOP_COUNT 5

// small cache parameters
#define INCACHE 25
#define MINWAIT (INCACHE+25)
#define REGFREQ 20

// error checking
#define check(e) \
(e ? (void)0 : (printf("%s:%u error: %s\n%s\n", __FILE__, __LINE__, #e, strerror(errno)), exit(1)))

// number of non-zero blocks in example ex
#define NUM_NONZERO(ex) (((int *)ex)[labelsize+1])

// float pointer to data segment of example ex
#define EX_DATA(ex) ((float *)(ex + sizeof(int)*(labelsize+3)))

// class label (+1 or -1) for the example
#define LABEL(ex) (((int *)ex)[1])

// block label (converted to 0-based index)
#define BLOCK_IDX(data) (((int)data[0])-1)

// set to 0 to use max-component L2 regularization
// set to 1 to use full model L2 regularization
#define FULL_L2 0

int labelsize;
int dim;


// comparison function for sorting examples 
int comp(const void *a, const void *b) {
  // sort by extended label first, and whole example second...
  int c = memcmp(*((char **)a) + sizeof(int), 
		 *((char **)b) + sizeof(int), 
		 labelsize*sizeof(int));
  if (c)
    return c;
  
  // labels are the same  
  int alen = **((int **)a);
  int blen = **((int **)b);
  if (alen == blen)
    return memcmp(*((char **)a) + sizeof(int), 
		  *((char **)b) + sizeof(int), 
		  alen);
  return ((alen < blen) ? -1 : 1);
}

// a collapsed example is a sequence of examples
struct collapsed {
  char **seq;
  int num;
};

// the two node types in an AND/OR tree
enum node_type { OR, AND };

// set of collapsed examples
struct data {
  collapsed *x;
  int num;
  int numblocks;
  int numcomponents;
  int *blocksizes;
  int *componentsizes;
  int **componentblocks;
  float *regmult;
  float *learnmult;
};

// seed the random number generator with an arbitrary (fixed) value
void seed_rand() {
#ifndef WIN
 srand48(3);
#else
 srand(3);
#endif
}

/*void load_fromfile(char *fn,double **w,data X)
{
    CvsMatrix<double> loader;
    loader.load(fn);
    int cn  =0;
    for (int j = 0; j < X.numblocks; j++) {
          for (int k = 0; k < X.blocksizes[j]; k++) {            
            w[j][k] = w_all[cn++];
         }
    }
}*/

static inline double min(double x, double y) { return (x <= y ? x : y); }
static inline double max(double x, double y) { return (x <= y ? y : x); }

// compute the score of an example
static inline double ex_score(const char *ex, data X, double **w) {
  double val = 0.0;
  float *data = EX_DATA(ex);
  int blocks = NUM_NONZERO(ex);
  for (int j = 0; j < blocks; j++) {
    int b = BLOCK_IDX(data);
    data++;
    double blockval = 0;
    for (int k = 0; k < X.blocksizes[b]; k++)
      blockval += w[b][k] * data[k];
    data += X.blocksizes[b];
    val += blockval;
  }
  return val;
}

// return the value of the object function.
// out[0] : loss on negative examples
// out[1] : loss on positive examples
// out[2] : regularization term's value
double compute_loss(double out[3], double C, double J, data X, double **w) {
  double loss = 0.0;
#if FULL_L2
  // compute ||w||^2
  for (int j = 0; j < X.numblocks; j++) {
    for (int k = 0; k < X.blocksizes[j]; k++) {
      loss += w[j][k] * w[j][k] * X.regmult[j];
    }
  }
#else
  // compute max norm^2 component
  for (int c = 0; c < X.numcomponents; c++) {
    double val = 0;
    for (int i = 0; i < X.componentsizes[c]; i++) {
      int b = X.componentblocks[c][i];
      double blockval = 0;
      for (int k = 0; k < X.blocksizes[b]; k++)
        blockval += w[b][k] * w[b][k] * X.regmult[b];
      val += blockval;
    }
    if (val > loss)
      loss = val;
  }
#endif
  loss *= 0.5;

  // record the regularization term
  out[2] = loss;

  // compute loss from the training data
  for (int l = 0; l <= 1; l++) {
    // which label subset to look at: -1 or 1
    int subset = (l*2)-1;
    double subsetloss = 0.0;
    for (int i = 0; i < X.num; i++) {
      collapsed x = X.x[i];

      // only consider examples in the target subset
      char *ptr = x.seq[0];
      if (LABEL(ptr) != subset)
        continue;

      // compute max over latent placements
      int M = -1;
      double V = -INFINITY;
      for (int m = 0; m < x.num; m++) {
        double val = ex_score(x.seq[m], X, w);
        if (val > V) {
          M = m;
          V = val;
        }
      }

      // compute loss on max
      ptr = x.seq[M];
      int label = LABEL(ptr);
      double mult = C * (label == 1 ? J : 1);
      subsetloss += mult * max(0.0, 1.0-label*V);
    }
    loss += subsetloss;
    out[l] = subsetloss;
  }
  return loss;
}

// gradient descent
void gd(double C, double J, data X, double **w, double **lb, char *logdir, char *logtag, int ITER, int MIN_ITER) {
  ofstream logfile;
  #ifdef WIN
  string filepath = string(logdir); 
  filepath.append("loc.log"); 
  char app = '\\';
  #else
  string filepath = string(logdir) + "/learnlog/" + string(logtag) + ".log";
  char app = '/';
  #endif

  logfile.open(filepath.c_str());
  logfile.precision(14);
  logfile.setf(ios::fixed, ios::floatfield);
  int num = X.num;
  int thr = (int)((double)(ITER)/50.0);
  int thr_sh = (int)((double)(ITER)/100.0);
  
  // state for random permutations
  int *perm = (int *)malloc(sizeof(int)*X.num);
  check(perm != NULL);

  // state for small cache
  int *W = (int *)malloc(sizeof(int)*num);
  check(W != NULL);
  for (int j = 0; j < num; j++)
    W[j] = INCACHE;

  double prev_loss = 1E9;
  bool converged = false;
  int stop_count = 0;
  int t = 0;
  double losses[10000];
  int cnl = 0;
  int messages_given  = 0;
  int sdims;

   while (t < ITER && !converged) {
    // pick random permutation
    for (int i = 0; i < num; i++)
      perm[i] = i;
    for (int swapi = 0; swapi < num; swapi++) {
      #ifdef WIN
      int swapj = (int)((double(rand())/(RAND_MAX+1))*(num-swapi)) + swapi;
      #else   
      int swapj = (int)(drand48()*(num-swapi)) + swapi;
      #endif
      int tmp = perm[swapi];
      perm[swapi] = perm[swapj];
      perm[swapj] = tmp;
    }

    // count number of examples in the small cache
    int cnum = 0;
    for (int i = 0; i < num; i++)
      if (W[i] <= INCACHE)
    	cnum++;

	int numupdated = 0;
    for (int swapi = 0; swapi < num; swapi++) {
      // select example
      int i = perm[swapi];
      // skip if example is not in small cache
      if (W[i] > INCACHE) {
        W[i]--;
        continue;
      }
      collapsed x = X.x[i];

      // learning rate
      double T = min(double(ITER)/2.0, t + 10000.0);
      double rateX = cnum * C / T;
	  
      t++;
	  int n_single = 10;
      if ((t<=n_single)||(t % thr_sh == 0)) {
        double info[3];
        double loss = compute_loss(info, C, J, X, w);

        double delta = 1.0 - (fabs(prev_loss - loss) / loss);
        logfile << t << "\t" << loss << "\t" << delta << endl;
        if (delta >= DELTA_STOP && t >= MIN_ITER) {
          stop_count++;
          if (stop_count > STOP_COUNT)
            converged = true;
        } else if (stop_count > 0) {
          stop_count = 0;
        }
        prev_loss = loss;
		losses[cnl++] = loss;

		if ((t<=n_single)||(t % thr == 0)) {
        printf("\r%7.2f%% of max #, it %i ms %i"
               "(delta = %.5f; loss = %.5f; stop count = %d)", 
               100*double(t)/double(ITER),t,messages_given, max(delta, 0.0),loss,
               STOP_COUNT - stop_count + 1);
		fflush(stdout);

        //-------------------------------------------------------------
        /* Some debugging code by Iasonas */
        //-------------------------------------------------------------
        /* Save weight vector at current iteration*/

        // store weight vector in w_all
        
		sdims = 0;
        for (int j = 0; j < X.numblocks; j++)
		{sdims = sdims + X.blocksizes[j];}
        float *w_all;
        w_all  =new float[sdims];
        int cn =0;
        for (int j = 0; j < X.numblocks; j++) {
          for (int k = 0; k < X.blocksizes[j]; k++) {            
            w_all[cn++] =  w[j][k];
          }
        }

        // and save it in file
		messages_given++;
		char str[300]  = "";
        char lbl[5]  = "wght";
        sprintf(str,"%s%s%i",logdir,lbl,messages_given);
		FILE *fp = fopen(str,"w");	
        if(fp)
    	{
		fprintf(fp,"%d %d\n",1,sdims);		
		{
			for(int j=0;j<sdims;j++)
			{
				fprintf(fp,"%1.10lf ",w_all[j]);
			}
			fprintf(fp,"\n");
		}
        fclose(fp);
        }
        delete []w_all;

        //------------------------------------------------------------
        /* Save score vector at current iteration*/
        //------------------------------------------------------------
        // compute scores
		double *scores; scores = new double[num];
        for (int i=0; i<num; i++)
        {
        // compute max over latent placements
        collapsed xc = X.x[i];
        int M = -1;
        double V = -INFINITY;
        for (int m = 0; m < xc.num; m++) {
            double val = ex_score(xc.seq[m], X, w);
            if (val > V) {
                M = m;
                V = val;
             }
          }
    	  scores[i] = V;
    	}
        // and save them in file
        char lbs[5]   = "scrs";
        char s0[300]  = "";
        sprintf(s0,"%s%s%i",logdir,lbs,messages_given);
    
    	FILE *fs = fopen(s0,"w");	
        if(fs)
    	{        
		fprintf(fs,"%d%d\n",1,num);		
		{
			for(int j=0;j<num;j++)
			{
				fprintf(fs,"%1.10lf ",scores[j]);
			}
			fprintf(fs,"\n");
		}
        fclose(fs);
        }

		delete []scores;
		
        //-------------------------------------------------------------
        /* end of debugging code by Iasonas */
        //-------------------------------------------------------------

        if (converged)
		{
		  printf("converged");
          break;
		}
		}
	  }


      // compute max over latent placements
      int M = -1;
      double V = -INFINITY;
      for (int m = 0; m < x.num; m++) {
        double val = ex_score(x.seq[m], X, w);
        if (val > V) {
            M = m;
            V = val;
         }
      }

      // and then, if support vector,  update weight vector 
      char *ptr = x.seq[M];
      int label = LABEL(ptr);
      if (label * V < 1.0) {
        numupdated++;
    	W[i] = 0;
        float *data = EX_DATA(ptr);
        int blocks = NUM_NONZERO(ptr);
		for (int j = 0; j < blocks; j++) {
			int b = BLOCK_IDX(data);
			double mult = (label > 0 ? J : -1) * rateX * X.learnmult[b];      
			data++;			
			for (int k = 0; k < X.blocksizes[b]; k++)
				w[b][k] += mult * data[k];
				data += X.blocksizes[b];
		}
	} 
	  else {
		if (W[i] == INCACHE)
		#ifdef WIN
			W[i] = MINWAIT + (int)((double(rand())/(RAND_MAX+1))*50);
		#else   
			W[i] = MINWAIT + (int)(drand48()*50);
		#endif
		else
		W[i]++;
      }

      // periodically regularize the model
      if (t % REGFREQ == 0) {
        // apply lowerbounds
        for (int j = 0; j < X.numblocks; j++)
          for (int k = 0; k < X.blocksizes[j]; k++)
            w[j][k] = max(w[j][k], lb[j][k]);
        double rateR = 1.0 / T;

#if FULL_L2 
        // update model
        for (int j = 0; j < X.numblocks; j++) {
          double mult = rateR * X.regmult[j] * X.learnmult[j];
          mult = pow((1-mult), REGFREQ);
          for (int k = 0; k < X.blocksizes[j]; k++) {
            w[j][k] = mult * w[j][k];
          }
        }
#else
        // assume simple mixture model
        int maxc = 0;
        double bestval = 0;
        for (int c = 0; c < X.numcomponents; c++) {
          double val = 0;
          for (int i = 0; i < X.componentsizes[c]; i++) {
            int b = X.componentblocks[c][i];
            double blockval = 0;
            for (int k = 0; k < X.blocksizes[b]; k++)
              blockval += w[b][k] * w[b][k] * X.regmult[b];
            val += blockval;
          }
          if (val > bestval) {
            maxc = c;
            bestval = val;
          }
        }
        for (int i = 0; i < X.componentsizes[maxc]; i++) {
          int b = X.componentblocks[maxc][i];
          double mult = rateR * X.regmult[b] * X.learnmult[b];        
          mult = pow((1-mult), REGFREQ);
          for (int k = 0; k < X.blocksizes[b]; k++)
            w[b][k] = mult * w[b][k];
		}
#endif
      }
    }
  }

  if (converged)
    printf("\nTermination criteria reached after %d iterations.\n", t);
  else
    printf("\nMax iteration count reached, t: %d.\n", t);

  free(perm);
  free(W);
  logfile.close();

  //------------------------------------------------------------
  // some debugging code by Iasonas
  //------------------------------------------------------------
  // convergence: store cost function as increasing iterations	
  
  char str[300]  = "";
  char lbl[5] = "ener";
  sprintf(str,"%s%s",logdir,lbl);
		FILE *fo = fopen(str,"w");	
        if(fo)
    	{        
		fprintf(fo,"%d%d\n",1,cnl);		
		{
			for(int j=0;j<cnl;j++)
			{
				fprintf(fo,"%1.10lf ",losses[j]);
			}
			fprintf(fo,"\n");
		}
        fclose(fo);
        }
}

// score examples
double *score(data X, char **examples, int num, double **w) {
  double *s = (double *)malloc(sizeof(double)*num);
  check(s != NULL);
  for (int i = 0; i < num; i++)
    s[i] = ex_score(examples[i], X, w);
  return s;  
}

// merge examples with identical labels
void collapse(data *X, char **examples, int num) {
  collapsed *x = (collapsed *)malloc(sizeof(collapsed)*num);
  check(x != NULL);
  int i = 0;
  x[0].seq = examples;
  x[0].num = 1;
  for (int j = 1; j < num; j++) {
    if (!memcmp(x[i].seq[0]+sizeof(int), examples[j]+sizeof(int), 
		labelsize*sizeof(int))) {
      x[i].num++;
    } else {
      i++;
      x[i].seq = &(examples[j]);
      x[i].num = 1;
    }
  }
  X->x = x;
  X->num = i+1;  
}

int main(int argc, char **argv) {  
  seed_rand();
   int count;
  data X;

  // command line arguments
  check(argc == 14);
  double C = atof(argv[1]);
  double J = atof(argv[2]);

  char *hdrfile = argv[3];
  char *datfile = argv[4];
  char *modfile = argv[5];
  char *inffile = argv[6];
  char *lobfile = argv[7];
  char *cmpfile = argv[8];
  char *objfile = argv[9];
  char *logdir  = argv[10];
  char *logtag  = argv[11];

  int ITER		= atoi(argv[12]);
  int MIN_ITER	= atoi(argv[13]);

  // read header file
  FILE *f = fopen(hdrfile, "rb");
  check(f != NULL);
  int header[3];
  count = fread(header, sizeof(int), 3, f);
  check(count == 3);
  int num = header[0];
  labelsize = header[1];
  X.numblocks = header[2];
  X.blocksizes = (int *)malloc(X.numblocks*sizeof(int));
  count = fread(X.blocksizes, sizeof(int), X.numblocks, f);
  check(count == X.numblocks);
  X.regmult = (float *)malloc(sizeof(float)*X.numblocks);
  check(X.regmult != NULL);
  count = fread(X.regmult, sizeof(float), X.numblocks, f);
  check(count == X.numblocks);
  X.learnmult = (float *)malloc(sizeof(float)*X.numblocks);
  check(X.learnmult != NULL);
  count = fread(X.learnmult, sizeof(float), X.numblocks, f);
  check(count == X.numblocks);
  check(num != 0);
  fclose(f);
  printf("%d examples with label size %d and %d blocks\n",
	 num, labelsize, X.numblocks);
  printf("block size, regularization multiplier, learning rate multiplier\n");
  dim = 0;
  for (int i = 0; i < X.numblocks; i++) {
    dim += X.blocksizes[i];
    printf("%d, %.2f, %.2f\n", X.blocksizes[i], X.regmult[i], X.learnmult[i]);
  }

  // read component info file
  // format: #components {#blocks blk1 ... blk#blocks}^#components
  f = fopen(cmpfile, "rb");
  count = fread(&X.numcomponents, sizeof(int), 1, f);
  check(count == 1);
  printf("the model has %d components\n", X.numcomponents);
  X.componentblocks = (int **)malloc(X.numcomponents*sizeof(int *));
  X.componentsizes = (int *)malloc(X.numcomponents*sizeof(int));
  for (int i = 0; i < X.numcomponents; i++) {
    count = fread(&X.componentsizes[i], sizeof(int), 1, f);
    check(count == 1);
    printf("component %d has %d blocks:", i, X.componentsizes[i]);
    X.componentblocks[i] = (int *)malloc(X.componentsizes[i]*sizeof(int));
    count = fread(X.componentblocks[i], sizeof(int), X.componentsizes[i], f);
    check(count == X.componentsizes[i]);
    for (int j = 0; j < X.componentsizes[i]; j++)
      printf(" %d", X.componentblocks[i][j]);
    printf("\n");
  }
  fclose(f);

  // read examples
  f = fopen(datfile, "rb");
  check(f != NULL);
  printf("Reading examples\n");
  char **examples = (char **)malloc(num*sizeof(char *));
  check(examples != NULL);
  #ifdef WIN
    int *buf; buf = new int[labelsize+2];
  #endif
  for (int i = 0; i < num; i++) {
    // we use an extra byte in the end of each example to mark unique
    // we use an extra int at the start of each example to store the 
    // example's byte length (excluding unique flag and this int)
    #ifndef WIN
        int buf[labelsize+2];
     #endif
    count = fread(buf, sizeof(int), labelsize+2, f);
    check(count == labelsize+2);
    // byte length of an example's data segment
    int len = sizeof(int)*(labelsize+2) + sizeof(float)*buf[labelsize+1];
    // memory for data, an initial integer, and a final byte
    examples[i] = (char *)malloc(sizeof(int)+len+1);
    check(examples[i] != NULL);
    // set data segment's byte length
    ((int *)examples[i])[0] = len;
    // set the unique flag to zero
    examples[i][sizeof(int)+len] = 0;
    // copy label data into example
    for (int j = 0; j < labelsize+2; j++)
      ((int *)examples[i])[j+1] = buf[j];
    // read the rest of the data segment into the example
    count = fread(examples[i]+sizeof(int)*(labelsize+3), 1, 
		  len-sizeof(int)*(labelsize+2), f);
    check(count == len-sizeof(int)*(labelsize+2));
  }
  #ifdef WIN
  delete[] buf;
  #endif
  fclose(f);
  printf("done\n");

  // sort
  printf("Sorting examples\n");
  char **sorted = (char **)malloc(num*sizeof(char *));
  check(sorted != NULL);
  memcpy(sorted, examples, num*sizeof(char *));
  qsort(sorted, num, sizeof(char *), comp);
  printf("done\n");

  // find unique examples
  int i = 0;
  int len = *((int *)sorted[0]);
  sorted[0][sizeof(int)+len] = 1;
  for (int j = 1; j < num; j++) {
    int alen = *((int *)sorted[i]);
    int blen = *((int *)sorted[j]);
    if (alen != blen || 
	memcmp(sorted[i] + sizeof(int), sorted[j] + sizeof(int), alen)) {
      i++;
      sorted[i] = sorted[j];
      sorted[i][sizeof(int)+blen] = 1;
    }
  }
  int num_unique = i+1;
  printf("%d unique examples\n", num_unique);

  // collapse examples
  collapse(&X, sorted, num_unique);
  printf("%d collapsed examples\n", X.num);

  // initial model
  double **w = (double **)malloc(sizeof(double *)*X.numblocks);
  check(w != NULL);
  f = fopen(modfile, "rb");
  for (int i = 0; i < X.numblocks; i++) {
    w[i] = (double *)malloc(sizeof(double)*X.blocksizes[i]);
    check(w[i] != NULL);
    count = fread(w[i], sizeof(double), X.blocksizes[i], f);
    check(count == X.blocksizes[i]);
  }
  fclose(f);

  //load_fromfile(char *fn,double **w,data X)
  // lower bounds
  double **lb = (double **)malloc(sizeof(double *)*X.numblocks);
  check(lb != NULL);
  f = fopen(lobfile, "rb");
  for (int i = 0; i < X.numblocks; i++) {
    lb[i] = (double *)malloc(sizeof(double)*X.blocksizes[i]);
    check(lb[i] != NULL);
    count = fread(lb[i], sizeof(double), X.blocksizes[i], f);
    check(count == X.blocksizes[i]);
  }
  fclose(f);
  
  // train
  printf("Training\n");
  gd(C, J, X, w, lb, logdir, logtag,ITER,MIN_ITER);
  printf("done\n");

  // save model
  printf("Saving model\n");
  f = fopen(modfile, "wb");
  check(f != NULL);
  for (int i = 0; i < X.numblocks; i++) {
    count = fwrite(w[i], sizeof(double), X.blocksizes[i], f);
    check(count == X.blocksizes[i]);
  }
  fclose(f);

  // score examples
  printf("Scoring\n");
  double *s = score(X, examples, num, w);

  // Write info file
  printf("Writing info file\n");
  f = fopen(inffile, "w");
  check(f != NULL);
  for (int i = 0; i < num; i++) {
    int len = ((int *)examples[i])[0];
    // label, score, unique flag
    count = fprintf(f, "%d\t%f\t%d\n", ((int *)examples[i])[1], s[i], 
                    (int)examples[i][sizeof(int)+len]);
    check(count > 0);
  }
  fclose(f);

  // compute loss and write it to a file
  double lossinfo[3];
  compute_loss(lossinfo, C, J, X, w);
  printf("Writing objective function info file\n");
  f = fopen(objfile, "w");
  count = fprintf(f, "%f\t%f\t%f", lossinfo[0], lossinfo[1], lossinfo[2]);
  check(count > 0);
  fclose(f);
  
  printf("Freeing memory\n");
  for (int i = 0; i < X.numblocks; i++) {
    free(w[i]);
    free(lb[i]);
  }
  free(w);
  free(lb);
  free(s);
  for (int i = 0; i < num; i++)
    free(examples[i]);
  free(examples);
  free(sorted);
  free(X.x);
  free(X.blocksizes);
  free(X.regmult);
  free(X.learnmult);
  for (int i = 0; i < X.numcomponents; i++)
    free(X.componentblocks[i]);
  free(X.componentblocks);
  free(X.componentsizes);

  return 0;
}