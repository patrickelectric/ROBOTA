class matlab
{
	private:
	FILE *file;                        //declara um arquivo para leitura e escrita
	static const int sizeclass=100;
	static const char* fileName()
    {
       return "MatlabInfo.m";
    };

	public:
	int i;
	int x[sizeclass];
	int y[sizeclass];
	float timexy[sizeclass];
	int erro[sizeclass];
	float theta[sizeclass];

	bool testLim()
	{
		if (i<=(int)(sizeclass)) return true;
		else
		{
			printf("LIMITE I DO MATLAB!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
			return false;
		}
	}
	
	void saveMatlab()
	{
		file = fopen(fileName(), "w");      //abre o arquivo para escrita
		fprintf(file, "x=[");
		for (int i=1; i<=(int)sizeof(x)/4;i++) fprintf(file,"%d,",x[i]);
		//fprintf(file,"\b");
		fprintf(file,"];\n");
		fprintf(file, "y=[");
		for (int i=1; i<=(int)sizeof(y)/4;i++) fprintf(file,"%d,",y[i]);
		//fprintf(file,"\b");
		fprintf(file,"];\n");
		fprintf(file, "timexy=[");
		for (int i=1; i<=(int)sizeof(timexy)/4;i++) fprintf(file,"%f,",timexy[i]);
		//fprintf(file,"\b");
		fprintf(file,"];\n");
		/*
		fprintf(file, "erro[%d]={",(int)sizeof(erro)/4);
		for (int i=1; i<=sizeof(erro)/4;i++) fprintf(file,"%d,",erro[i]);
		//fprintf(file,"\b");
		fprintf(file,"};\n");
		*/
		fprintf(file, "theta=[");
		for (int i=1; i<=(int)sizeof(theta)/4;i++) fprintf(file,"%f,",theta[i]);
		//fprintf(file,"\b");
		fprintf(file,"];\n");
		fclose(file);  
	}

};

matlab matlab;