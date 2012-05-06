#include "EvaluationEngine.h"
#include "qmath.h"
#include "narx_util.h"


EvaluationEngine::EvaluationEngine(int slen)
{
	series_len = slen;
	series = new double[slen];
	predicted = new double[slen];


	curlen = 0;
}


EvaluationEngine::~EvaluationEngine(void)
{
	delete [] series;
	delete [] predicted;
}

void EvaluationEngine::insertvalue(double target, double pred)
{
	series[curlen]=target;
	predicted[curlen]=pred;
	curlen++;
}

double EvaluationEngine::F3()
{
	double ret = 0;
	double sum = 0;
	for(int i=0;i<curlen;i++)
	{
		ret += qSqrt(qAbs(series[i]*series[i] - predicted[i]*predicted[i]));
		
		sum += series[i];
	}

	return ret/sum;
	
}


double EvaluationEngine::Fd(double val)
{
	double c=0;
	for(int i=0;i<curlen;i++)
		if(series[i]<=val) c++;
	return c/curlen;
}

double EvaluationEngine::Fy(double val)
{
	double c=0;
	for(int i=0;i<curlen;i++)
		if(predicted[i]<=val) c++;
	return c/curlen;
}

double EvaluationEngine::KS1()
{
	double max = 0;
	for(int i=0;i<curlen;i++)
	{
			double t = qAbs(Fd(series[i]) - Fy(series[i]));
			if(max < t) max = t;
	}

	return max;
}

double EvaluationEngine::KS2()
{
	double max = 0;
	for(int i=0;i<curlen;i++)
	{
			double t = qAbs(Fd(predicted[i]) - Fy(predicted[i]));
			if(max < t) max = t;
	}
	return max;
}

double EvaluationEngine::KS12()
{
	double max = 0;
	for(int i=0;i<curlen;i++)
	{
			double t = qAbs(Fd(series[i]) - Fy(predicted[i]));
			if(max < t) max = t;
	}
	return max;
}

double EvaluationEngine::DA()
{
	double ret=0;
	for(int i=0;i<curlen;i++) {
		ret+=(2*i+1)/curlen * ( qLn(Fd(predicted[i])) + qLn(Fd(predicted[curlen-1-i])));
		////char test[256];
		//sprintf(test, "aa %f %f\n", Fd(predicted[i]),(2*i+1)/curlen * ( qLn(Fd(predicted[i])) + qLn(Fd(predicted[curlen-1-i]))));
		//FLOG(test);
	}
	return - curlen - ret;
}

void EvaluationEngine::reset()
{
	//for( int i=0;i<series_len;i++)
	//	series[i]=predicted[i]=0;
	curlen = 0;
}