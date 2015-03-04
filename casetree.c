int evaltree2(struct node * nd,int i){
	if (nd==NULL) {	
		return 1;
	}
	//print(nd);
	int n,size,read_val,place,t,tempe,num;
	char tmp;
	struct gnode * temp;
	switch (nd->flag){

		case _INT : return nd->val;
		
		case '+': 	return (evaltree2(nd->left,i) + evaltree2(nd->right,i));
		
		case '-':	return (evaltree2(nd->left,i) - evaltree2(nd->right,i));
		
		case '*':	return (evaltree2(nd->left,i) * evaltree2(nd->right,i));
		
		case '/':	return (evaltree2(nd->left,i) / evaltree2(nd->right,i));
		
		case '>':
					if (evaltree2(nd->left,i) > evaltree2(nd->right,i)) return 1;
	 				else return 0;
	 	
	 	case '<':
	 				if (evaltree2(nd->left,i) < evaltree2(nd->right,i)) return 1;
	 				else return 0;

	 	case _EQEQ:
	 				if (evaltree2(nd->left,i) == evaltree2(nd->right,i)) return 1;
		 			else return 0;
		
		case _Truth:return nd->val;

	 	case '=':
					t=evaltree2(nd->right,i);
					struct gnode* temp=fetch(nd->left->varname);
				
					if(nd->right->flag == '>' || nd->right->flag == '<' || nd->right->flag == _EQEQ || nd->right->flag == _Truth){
							
							if(temp->type !=1) {
								printf("int - bool TYPE MISMATCH\n");
								exit(1); ///for the time being //later add line no.
							}
							else{
								if(nd->left->flag==_ID)
									set(nd->left->varname,t,0); //set
								else if(nd->left->flag==_ARRAY){
									place = evaltree2(nd->left->left,i);
									set(nd->left->varname,t,place);
								}
							}
					}
					else{
						if(temp->type !=0) {
							printf("bool - int TYPE MISMATCH\n");
							exit(1);
						}
						else{
						//printf("%s = evaluated val : %d\n",nd->left->varname,t);
							if(nd->left->flag==_ID)
								set(nd->left->varname,t,0); //set
							else if(nd->left->flag==_ARRAY){
								place = evaltree2(nd->left->left,i);
								set(nd->left->varname,t,place);
							}
						}
					}

	 	case _Program:
			 		evaltree2(nd->left,i);
					evaltree2(nd->right,i);

		case _GDefList:
					evaltree2(nd->left,i);
					evaltree2(nd->right,i);

		case _GINT:
					evaltree2(nd->left,0);

		case _GBOOL:
					evaltree2(nd->left,1);

	 	case _ID:
	 		//struct gnode * temp;
					temp=fetch(nd->varname);
					num= *(temp->bind);
					return num;

		case _ARRAY:
					place=evaltree2(nd->left,i);
					temp=fetch(nd->varname);
					num= *(temp->bind+place);
					return num;
	 	
	 	case _Varlist:
			 		evaltree2(nd->left,i);
					if(nd->right->flag==_ID) {
						gentry(nd->right->varname,i,1);
						return 1;
					}
					else if(nd->right->flag==_ARRAY){
						size=evaltree2(nd->right->left,i);
						gentry(nd->right->varname,i,size);
						return 1;
					}


		case _INTD:
					evaltree(nd->left,0);

		case _READ:
					tempe;
					temp=fetch(nd->left->varname);
					if(temp->type==0){
						
						printf("Enter a number : ");
						scanf("%d",&tempe);
						
					}
					else{
						tmp;
						printf("Enter T or F :");
						scanf("%c",&tmp);
						if(tmp=='T' || tmp=='t') tempe=1;
						else if(tmp=='F'|| tmp=='f') tempe=0;
						else{
							printf("bool - int type mismatch (wrong input)\n");
							exit(1);

						}
					}
					if(nd->left->flag==_ID)
						set(nd->left->varname,tempe,0); //set
					else{
						place = evaltree2(nd->left->left,i);
						set(nd->left->varname,tempe,place);
					}
					printf("reading done\n");
				
		case _WRITE:
					temp=fetch(nd->left->varname);
					if(temp->type==1)
						printf("Bool : ");

					printf("printing %d\n",evaltree2(nd->left,i));
		
		case _IF:
				 	n=evaltree2(nd->left,i);
					if(n==1) evaltree2(nd->right,i);
		
		case _WHILE:
					while(evaltree2(nd->left,i)){
						evaltree2(nd->right,i);
					}
			//printf(" ");
		case _StmtList :
					evaltree2(nd->left,i);
					evaltree2(nd->right,i);

	}
	return 0;
}

