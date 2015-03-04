int evaltree(struct node* nd,int i){		//infix eval
	if (nd == NULL) {	
		return 1;
	}
	//print(nd);
	if(nd->flag==INT){		//integer
		return nd->val;
	}	

	//check both sides as integer
	if (nd->flag=='+')
		return (evaltree(nd->left,i) + evaltree(nd->right,i));

	else if(nd->flag== '*')
		return (evaltree(nd->left,i) * evaltree(nd->right,i));
	
	else if(nd->flag=='/')
	 	return (evaltree(nd->left,i) / evaltree(nd->right,i));
	
	else if(nd->flag=='-')
	 	{	printf("%d - %d\n",evaltree(nd->left,i) ,evaltree(nd->right,i) );
	 		int dog= (evaltree(nd->left,i) - evaltree(nd->right,i));
	 		printf("dog = %d\n",dog);
	 		return dog;
	 	}

	else if(nd->flag==_mod)
	 	{	printf("%d mod %d\n",evaltree(nd->left,i) ,evaltree(nd->right,i) );
	 		int dog= (evaltree(nd->left,i) % evaltree(nd->right,i));
	 		printf("dog = %d\n",dog);
	 		return dog;
	 	}


	//revission needed whather to return 1/0 or t/f  time : 5:18  3/3/15 
	else if(nd->flag=='>'){
		if (evaltree(nd->left,i) > evaltree(nd->right,i)) return TRUE;
	 	else return FALSE;
	}
	else if(nd->flag=='<'){
	 	if (evaltree(nd->left,i) < evaltree(nd->right,i)) return TRUE;
	 	else return FALSE;
	}

	else if(nd->flag==EQEQ){
	 	if (evaltree(nd->left,i) == evaltree(nd->right,i)) return TRUE;
	 	else return FALSE;
	}


	//later added
	else if(nd->flag==NE){
	 	if (evaltree(nd->left,i)!=evaltree(nd->right,i)) return TRUE;
	 	else return FALSE;
	}
	else if(nd->flag==GE){
	 	if (evaltree(nd->left,i)>= evaltree(nd->right,i)) return TRUE;
	 	else return FALSE;
	}
	else if(nd->flag==LE){
	 	if (evaltree(nd->left,i) <= evaltree(nd->right,i)) return TRUE;
	 	else return FALSE;
	}



	//checking for bool (and. or .not . )
	if(nd->flag==AND){
		if (evaltree(nd->left,i) == TRUE  && evaltree(nd->right,i) == TRUE )
		return TRUE;
		else return FALSE;
	}
	else if(nd->flag==OR){
		if (evaltree(nd->left,i) == FALSE  && evaltree(nd->right,i) == FALSE )
		return FALSE;
		else return TRUE;
	}
	else if(nd->flag==NOT){
	 	if (evaltree(nd->left,i) == TRUE )
		return FALSE;
		else return TRUE;
	}

	
	else if(nd->flag==_Truth){
			return nd->val;
	}


	else if(nd->flag== '='){
		int t=evaltree(nd->right,i);			
			
		if(nd->left->flag==ID)   set(nd->left->varname,t,0);

		else if(nd->left->flag==ARRAY){
			int place = evaltree(nd->left->left,i);
			set(nd->left->varname,t,place);
		}


	}
	

	else if(nd->flag==_Program){
		evaltree(nd->left,i);
		evaltree(nd->right,i);
	}

	else if(nd->flag==_GDefList){
		evaltree(nd->left,i);
		evaltree(nd->right,i);
	}
	else if(nd->flag==GINT){	//to declaration
		evaltree(nd->left,0);	//important type here

	}
	else if(nd->flag==GBOOL){	//here too----------
		evaltree(nd->left,1);	
	}

	else if(nd->flag==ID){		//getter
		//printf("test@identi : %s\n",need->varname);
		struct gnode * temp;
		temp=fetch(nd->varname);
		int num= *(temp->bind);
		return num;

	}

	else if(nd->flag==ARRAY){
		struct gnode * temp;
		int place=evaltree(nd->left,i);
		temp=fetch(nd->varname);
		int num= *(temp->bind+place);
		return num;
		
	}

	else if(nd->flag==_Varlist){
		evaltree(nd->left,i);
		
		if(nd->right->flag==ID) {
			gentry(nd->right->varname,i,1);
			return 1;
		}
		
		else if(nd->right->flag==ARRAY){
			int size=evaltree(nd->right->left,i);
			gentry(nd->right->varname,i,size);
			return 1;
		}
	}

	else if(nd->flag==INTD){
		evaltree(nd->left,0);				//type =0 for integers
				
	}
	else if(nd->flag==READ){				//alpha : need to check
		
		int temp;	
		printf("Enter a number : ");
		scanf("%d",&temp);

		if(nd->left->flag==ID)
			set(nd->left->varname,temp,0); //set
		else{
			int place = evaltree(nd->left->left,i);
			set(nd->left->varname,temp,place);
		}
		printf("reading done\n");
		
	}

	else if(nd->flag==WRITE){

		printf("printing %d\n",evaltree(nd->left,i));

	}

	else if(nd->flag==IF){	
		
		if (evaltree(nd->left,i)==TRUE) evaltree(nd->right,i);
		else return 1;
	}

	else if(nd->flag==WHILE){

		while(evaltree(nd->left,i) == TRUE){
			evaltree(nd->right,i);
		}

	}

	else if(nd->flag == _StmtList){

		evaltree(nd->left,i);
		evaltree(nd->right,i);

	}

	return 1;
}