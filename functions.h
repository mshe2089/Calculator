struct Algebraic {//Struct to hold a composite algrbraic term
    int Top;
    double* powers;
    double* values;
};

int verify_equation(char* equation_In, int equation_Len);//Checks if equation is valid
void solve_equation(char* equation_In);//Calculates the answer from a valid equation string. CHANGE TO RETURN!!
void compact_equation(char* equation_In, int equation_Len, int* add_equation_Len);//Removes whitespace from string.
char pop(char* holding_String, int* holding_Top);//Pops a char from a char stack
void push(char* holding_String, int* holding_Top, char character);//Pushes a char into a char stack
int find(double* list, double target, int length);//Finds the index of a double in a double array. returns -1 if not found.
struct Algebraic equate(struct Algebraic thing1, struct Algebraic thing2);//Replaces thing1 contents with thing2's.
struct Algebraic reset(struct Algebraic thing1);//Resets struct algebraic thing1 to default values.
void dump(struct Algebraic thing1);//Prints contents of struct algebraic.

//=====================================================================================
void compact_equation(char* equation_In, int equation_Len, int* add_equation_Len)//Removes whitespace from string
{
    int i = 0;
    //Looping through every character in equation string
    while (i < equation_Len)
    {
        //If character is a whitespace, shuffle every character after space back by 1 (remove space)
        if (isspace(equation_In[i]) != 0)
        {
            for (int j = i; j < strlen(equation_In) -1; j++)
            {
                equation_In[j] = equation_In[j + 1];     
            }
            i--;
            equation_Len--;
        }
        i++;
    }
    //Terminate string and save new equation length
    equation_In[equation_Len+1] = '\0';
    *add_equation_Len = equation_Len;
}

//=====================================================================================
int verify_equation(char* equation_In, int equation_Len)//Checks if equation is valid
{
    //If a character is preceded by a non allowed character, print error and terminate program
    for (int i = 1; i < equation_Len; i++)
    {
        if (equation_In[i] == '+')
        {
            if (equation_In[i-1] == '.' || +
                equation_In[i-1] == '-' || +
                equation_In[i-1] == '*' || +
                equation_In[i-1] == '/' || +
                equation_In[i-1] == '^' || +
                equation_In[i-1] == '+' || +
                equation_In[i-1] == 'e' || +
                i == equation_Len - 1)
            {
                printf("Error: Illegal input!\n");
                return 0;
            }
        }
        else if (equation_In[i] == '*')
        {
            if (equation_In[i-1] == '.' || +
                equation_In[i-1] == '-' || +
                equation_In[i-1] == '*' || +
                equation_In[i-1] == '/' || +
                equation_In[i-1] == '^' || +
                equation_In[i-1] == '+' || +
                equation_In[i-1] == 'e' || +
                i == equation_Len - 1)
            {
                printf("Error: Illegal input!\n");
                return 0;
            }
        }
        else if (equation_In[i] == '/')
        {
            if (equation_In[i-1] == '.' || +
                equation_In[i-1] == '-' || +
                equation_In[i-1] == '*' || +
                equation_In[i-1] == '/' || +
                equation_In[i-1] == '^' || +
                equation_In[i-1] == '+' || +
                equation_In[i-1] == 'e' || +
                i == equation_Len - 1)
            {
                printf("Error: Illegal input!\n");
                return 0;
            }
        }
        else if (equation_In[i] == '^')
        {
            if (equation_In[i-1] == '.' || +
                equation_In[i-1] == '-' || +
                equation_In[i-1] == '*' || +
                equation_In[i-1] == '/' || +
                equation_In[i-1] == '^' || +
                equation_In[i-1] == '+' || +
                equation_In[i-1] == 'e' || +
                i == equation_Len - 1)
            {
                printf("Error: Illegal input!\n");
                return 0;
            }
        }
        else if (equation_In[i] == 'e')
        {
            if (equation_In[i-1] == '.' || +
                equation_In[i-1] == '-' || +
                equation_In[i-1] == '*' || +
                equation_In[i-1] == '/' || +
                equation_In[i-1] == '^' || +
                equation_In[i-1] == '+' || +
                equation_In[i-1] == 'e' || +
                i == equation_Len - 1)
            {
                printf("Error: Illegal input!\n");
                return 0;
            }
        }
        else if (equation_In[i] == '.')
        {
            if (equation_In[i-1] == '.' || +
                equation_In[i-1] == '-' || +
                equation_In[i-1] == '*' || +
                equation_In[i-1] == '/' || +
                equation_In[i-1] == '^' || +
                equation_In[i-1] == '+' || +
                equation_In[i-1] == 'e' || +
                i == equation_Len - 1)
            {
                printf("Error: Illegal input!\n");
                return 0;
            }
        }
    }
    //Check the balance of brackets, using a signed and unsigned int
    // ( = both ints +1, ) = both ints -1
    //balanced brackets should result in 0 for both ints at the end.
    //Print error and terminate otherwise.
    int order_balance = 0;
    int quant_balance = 0;
    for (int i = 0; i < equation_Len; i++)
    {
        if (equation_In[i] == '(')
        {
            order_balance ++;
            quant_balance ++;
        }
        else if (equation_In[i] == ')')
        {
            if (order_balance > 0)
            {
                order_balance --;
            }
            quant_balance --;
        }
    }
    if (order_balance != 0 || quant_balance != 0)
    {
        printf("Error: Illegal input!\n");
        return 0;
    }
    //Return 1 (success) when all checks pass
    return 1;  
}

//=====================================================================================
void solve_equation(char* equation_In)//Calculates the answer from a valid equation string.
{
    //:::::::::::::::::::::::::::
    //Example formula: 
    //(-x^2+1e3)6
    //:::::::::::::::::::::::::::

    //Defines opening variables
    char character; //Character type
    char allowed_chars[] = {'+','-','*','/','^','(',')'};
    int scientific = 0; //Scientific notation tracker
    int stack_Top = 0; //Top of all 3 stacks
    int holding_Top = 0; //Top of holding stack
    char* holding_String = (char*)calloc(100, sizeof(char)); //Static 100 long string to hold a number as it is being scanned.
    char* sign_Stack = (char*)calloc(100, sizeof(char)); //Static 100 long array for storing the signs of the equation
    double* num_Stack = (double*)calloc(100, sizeof(double)); //Static 100 long array for storing the numeric values of the equation
    double* alg_Stack = (double*)calloc(100, sizeof(int)); //Static 100 long array for storing the algebraic values of the equation

    for (int i = 0; i < strlen(equation_In); i++) //For each character in equation string
    { 
        // Assign a letter to the current character, to indicate its "type"

        character = 'U';//unknown char
        if (i == strlen(equation_In)-1)
        {
            character = 'T';//last character(string terminator \0)
        }
        else if (isdigit(equation_In[i]) != 0 || equation_In[i] == '.')
        {
            character = 'N';//number char
        }
        else if (equation_In[i] == 'x')
        {
            character = 'X';//algebra char
        }
        else if (equation_In[i] == 'e')
        {
            character = 'S';//scientific char
        }
        else
        {
            for (int j = 0; j < strlen(allowed_chars); j++)
            {
                if (equation_In[i] == allowed_chars[j])
                {
                    character = 'A';//operator char
                }
            }
        }

        //Depending on character type, perform a certain action
        //NOTE: an 'X' in the operator stack indicates no operator in that position (in other words a numeric/algrbraic in that position)
        switch (character) 
        {
            case 'N': //number
                //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Push implicit multiplication if necessary (i think this is an invalid case?)
                if (sign_Stack[stack_Top - 1] == ')' && holding_Top == 0)//implicit multiplier for symbol )
                {
                    num_Stack[stack_Top] = 0;
                    alg_Stack[stack_Top] = 0;
                    sign_Stack[stack_Top] = '*';
                    stack_Top++;
                }
                //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Push number into holding string
                holding_String[holding_Top] = equation_In[i];
                holding_Top++;
                break;

            case 'X': //algebra
                //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Push number in holding string to stack
                if (holding_Top != 0)//THERE IS A NUMBER IN THE HOLDING STRING
                {
                    num_Stack[stack_Top] = atof(holding_String);
                    alg_Stack[stack_Top] = 0;
                    sign_Stack[stack_Top] = 'X';
                    for (int j = 0; j < holding_Top; j++)
                    {
                        holding_String[j] = '\0';   //CLEAR HOLDING
                    }
                    holding_Top = 0;
                    stack_Top++;
                }
                //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Push enotation bracket
                if (scientific == 1)//close bracket if flag is up
                {
                    scientific = 0;
                    num_Stack[stack_Top] = 0;
                    alg_Stack[stack_Top] = 0;
                    sign_Stack[stack_Top] = ')';//ENOTATION CLOSING BRACKET
                    stack_Top++;
                }
                //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Push implicit multiplication 
                if (sign_Stack[stack_Top - 1] == ')' || sign_Stack[stack_Top - 1] == 'X')//implicit multiplier if last symbol was number or )
                {
                    num_Stack[stack_Top] = 0;
                    alg_Stack[stack_Top] = 0;
                    sign_Stack[stack_Top] = '*';
                    stack_Top++;
                }
                //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Push algebra
                num_Stack[stack_Top] = 1;
                alg_Stack[stack_Top] = 1;
                sign_Stack[stack_Top] = 'X';
                stack_Top++;
                break;

            case 'S': //scientific
                //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Push bracket and raise flag
                scientific = 1;
                num_Stack[stack_Top] = 0;
                alg_Stack[stack_Top] = 0;
                sign_Stack[stack_Top] = '(';
                stack_Top++;
                //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Push holding num to stack
                if (holding_Top != 0)//THERE IS A NUMBER IN THE HOLDING STRING
                {
                    num_Stack[stack_Top] = atof(holding_String);
                    alg_Stack[stack_Top] = 0;
                    sign_Stack[stack_Top] = 'X';
                    for (int j = 0; j < holding_Top; j++)
                    {
                        holding_String[j] = '\0';   //CLEAR HOLDING
                    }
                    holding_Top = 0;
                    stack_Top++;
                }
                //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Push enotation operators (xey = x*10^y)
                num_Stack[stack_Top] = 0;
                alg_Stack[stack_Top] = 0;
                sign_Stack[stack_Top] = '*';
                stack_Top++;
                num_Stack[stack_Top] = 10;
                alg_Stack[stack_Top] = 0;
                sign_Stack[stack_Top] = 'X';
                stack_Top++;
                num_Stack[stack_Top] = 0;
                alg_Stack[stack_Top] = 0;
                sign_Stack[stack_Top] = '^';
                stack_Top++;
                break;

            case 'A': //Symbol
                //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Push holding num to stack
                if (holding_Top != 0)//THERE IS A NUMBER IN THE HOLDING STRING
                {
                    num_Stack[stack_Top] = atof(holding_String);
                    if (alg_Stack[stack_Top] != 1)
                    {
                        alg_Stack[stack_Top] = 0;
                    }
                    sign_Stack[stack_Top] = 'X';

                    for (int j = 0; j < holding_Top; j++)
                    {
                        holding_String[j] = '\0';   //CLEAR HOLDING
                    }
                    holding_Top = 0;

                    stack_Top++;
                }   
                //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Push enotation bracket
                if (scientific == 1 && sign_Stack[stack_Top-1] == 'X')//close bracket if flag is up and current symbol is not a unary minus (preceding a numeric)
                {
                    scientific = 0;
                    num_Stack[stack_Top] = 0;
                    alg_Stack[stack_Top] = 0;
                    sign_Stack[stack_Top] = ')';//ENOTATION CLOSING BRACKET
                    stack_Top++;
                }
                //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Push implicit multiplication to stack
                if ((equation_In[i] == '(' && (sign_Stack[stack_Top - 1] == 'X' || sign_Stack[stack_Top - 1] == ')')))//multiplier for symbol (
                {
                    num_Stack[stack_Top] = 0;
                    alg_Stack[stack_Top] = 0;
                    sign_Stack[stack_Top] = '*';
                    stack_Top++;
                }
                //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Push operator to stack
                num_Stack[stack_Top] = 0;
                alg_Stack[stack_Top] = 0;
                sign_Stack[stack_Top] = equation_In[i];
                stack_Top++;
                break;
                
            case 'T': //end
                //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Push holding num to stack
                if (holding_Top != 0)//THERE IS A NUMBER IN THE HOLDING STRING
                {
                    num_Stack[stack_Top] = atof(holding_String);
                    alg_Stack[stack_Top] = 0;
                    sign_Stack[stack_Top] = 'X';
                    stack_Top++;
                }  
                //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Push enotation bracket
                if (scientific == 1)//close bracket if flag is up
                {
                    scientific = 0;
                    num_Stack[stack_Top] = 0;
                    alg_Stack[stack_Top] = 0;
                    sign_Stack[stack_Top] = ')';//ENOTATION CLOSING BRACKET
                    stack_Top++;
                }
                for (int j = 0; j < holding_Top; j++)
                {
                    holding_String[j] = '\0';   //CLEAR HOLDING
                }
                holding_Top = 0;
                break;

            case 'U': //Unknown character
                printf("Error: Illegal input!\n"); //Throw error and terminate
                return;
        }
    }

    //IDENTIFYING UNARY MINUSES
    for (int i = 0; i < stack_Top; i++)
    {
        //If a minus is preceded by an operator, it can be assumed to be an unary minus
        if (sign_Stack[i] == '-' && +
        (sign_Stack[i-1] == '('||+
        sign_Stack[i-1] == '*'||+
        sign_Stack[i-1] == '/'||+
        sign_Stack[i-1] == '+'||+
        sign_Stack[i-1] == '-'||+
        sign_Stack[i-1] == '^'||+
        i == 0))
        {
            sign_Stack[i] = 'I';//replace it with a special character, I
        }
    }

    //::::::::::::::::::::::::::::::::::::::::
    //What example formula looks like right now
    //op_stack
    //[(,I,X,^,X,+,(,X,*,X,^,X,),),*,X]
    //num_stack
    //[0,0,1,0,2,0,0,1,0,10,0,3,0,0,0,6]
    //alg_stack
    //[0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0]
    //::::::::::::::::::::::::::::::::::::::::

    //Defining new arrays for next step. Equation will be postfixed in the next step
    //and simulaneously, the algebraic/numeric terms will be converted into structs.
    int Post_stack_Top = 0; //Top of both sign and num stack
    char* Post_sign_Stack = (char*)malloc(stack_Top * sizeof(char)); //Stack of operators
    struct Algebraic* Post_num_Stack = (struct Algebraic*)malloc(stack_Top * sizeof(struct Algebraic)); //Stack of non operators (mixed algebraic/numeric terms)

    //::::::::::::::::::::::::::::::::::::::::::::
    //STRUCT ALGEBRAIC
    //Example mixed algebraic term:
    //6+3x+9x^2

    //What the struct version looks like
    //powers: [0,1,2] (powers of x)
    //values: [6,3,9] (amount of that power)
    //top: 3 (number of terms)
    //::::::::::::::::::::::::::::::::::::::::::::

    //====================================================POSTFIXING
    for (int i = 0; i < stack_Top; i++) //For all the operators in the operator stack from previous step
    {
        if (sign_Stack[i] != 'X') //If it is an operator
        {
            //Pop all operators in the holding string with an equal/higher precidence into post_sign_stack, 
            //then push the current operator into holding string.
            switch (sign_Stack[i]) 
            {   
                case '+':
                    while (holding_String[holding_Top-1] == '+' || +
                    holding_String[holding_Top-1] == '-' || +
                    holding_String[holding_Top-1] == '*' || +
                    holding_String[holding_Top-1] == '/' || +
                    holding_String[holding_Top-1] == '^' || +
                    holding_String[holding_Top-1] == 'I')
                    {
                        Post_sign_Stack[Post_stack_Top] = pop(holding_String, &holding_Top);
                        Post_num_Stack[Post_stack_Top] = reset(Post_num_Stack[Post_stack_Top]);
                        Post_stack_Top++;
                    }
                    push(holding_String, &holding_Top, '+');
                    break;

                case '-':
                    while (holding_String[holding_Top-1] == '+' || +
                    holding_String[holding_Top-1] == '-' || +
                    holding_String[holding_Top-1] == '*' || +
                    holding_String[holding_Top-1] == '/' || +
                    holding_String[holding_Top-1] == '^' || +
                    holding_String[holding_Top-1] == 'I')
                    {
                        Post_sign_Stack[Post_stack_Top] = pop(holding_String, &holding_Top);
                        Post_num_Stack[Post_stack_Top] = reset(Post_num_Stack[Post_stack_Top]);
                        Post_stack_Top++;
                    }
                    push(holding_String, &holding_Top, '-');
                    break;
                   
                case '*':
                    while (holding_String[holding_Top-1] == '*' || +
                    holding_String[holding_Top-1] == '/' || +
                    holding_String[holding_Top-1] == '^' || +
                    holding_String[holding_Top-1] == 'I')
                    {
                        Post_sign_Stack[Post_stack_Top] = pop(holding_String, &holding_Top);
                        Post_num_Stack[Post_stack_Top] = reset(Post_num_Stack[Post_stack_Top]);
                        Post_stack_Top++;
                    }
                    push(holding_String, &holding_Top, '*');
                    break;

                case '/':
                    while (holding_String[holding_Top-1] == '*' || +
                    holding_String[holding_Top-1] == '/' || +
                    holding_String[holding_Top-1] == '^' || +
                    holding_String[holding_Top-1] == 'I')
                    {
                        Post_sign_Stack[Post_stack_Top] = pop(holding_String, &holding_Top);
                        Post_num_Stack[Post_stack_Top] = reset(Post_num_Stack[Post_stack_Top]);
                        Post_stack_Top++;
                    }
                    push(holding_String, &holding_Top, '/');
                    break;
                    
                case '^':
                    while (holding_String[holding_Top-1] == '^'|| +
                    holding_String[holding_Top-1] == 'I')
                    {
                        Post_sign_Stack[Post_stack_Top] = pop(holding_String, &holding_Top);
                        Post_num_Stack[Post_stack_Top] = reset(Post_num_Stack[Post_stack_Top]);
                        Post_stack_Top++;
                    }
                    push(holding_String, &holding_Top, '^');
                    break;

                case ')':
                    while (holding_String[holding_Top-1] != '(')
                    {
                        Post_sign_Stack[Post_stack_Top] = pop(holding_String, &holding_Top);
                        Post_num_Stack[Post_stack_Top] = reset(Post_num_Stack[Post_stack_Top]);
                        Post_stack_Top++;
                    }
                    pop(holding_String, &holding_Top);
                    break;

                case '(':
                    push(holding_String, &holding_Top, '(');
                    break;

                case 'I':
                    while (holding_String[holding_Top-1] == 'I')
                    {
                        Post_sign_Stack[Post_stack_Top] = pop(holding_String, &holding_Top);
                        Post_num_Stack[Post_stack_Top] = reset(Post_num_Stack[Post_stack_Top]);
                        Post_stack_Top++;
                    }
                    push(holding_String, &holding_Top, 'I');
                    break;
                    
            }
        }
        else
        {
            //Convert number/algebra into struct format and push into post_num_stack
            Post_num_Stack[Post_stack_Top] = reset(Post_num_Stack[Post_stack_Top]);
            Post_num_Stack[Post_stack_Top].values[0] = num_Stack[i];
            Post_num_Stack[Post_stack_Top].powers[0] = alg_Stack[i];
            Post_sign_Stack[Post_stack_Top] = 'X';
            Post_stack_Top++;
        }
    }

    //Pop remaining operators in holding string
    while (holding_Top != 0)
    {
        Post_sign_Stack[Post_stack_Top] = pop(holding_String, &holding_Top);
        Post_num_Stack[Post_stack_Top] = reset(Post_num_Stack[Post_stack_Top]);
        Post_stack_Top++;
    }

    //Free unused arrays
    free(sign_Stack);
    free(num_Stack);
    free(alg_Stack);

    //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    //What formula looks like right now
    //Post_op_stack
    //[X,X,^,-,X,X,X,^,*,+,X,*]
    //Post_num_stack
    //[S,S,B,B,S,S,S,B,B,B,S,B]
    //An array of structs, each representing a mixed algebraic term.
    //eg. (1+4x+8x^2)
    //(S = a struct)
    //(B = a blank struct)

    //Right now they are all 'singular terms' ie. only contain either a number or a singular x 
    //eg. 68 or 12 or 7 or x.

    //But in the next step, as we calculate, we will begin storing mixed terms in them.
    //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    //Define new arrays
    struct Algebraic operand1;
    struct Algebraic operand2;
    struct Algebraic operand3;//these operands are used to temporarily store operands for calculation.

    //========================================================================================solving equation
    int i = 0;

    while (i < Post_stack_Top)
    {
        if (Post_sign_Stack[i] != 'X')//if operator encountered
        {
            //reset operands
            operand1 = reset(operand1);

            operand2 = reset(operand2);

            operand3 = reset(operand3);

            switch (Post_sign_Stack[i])//Depending on operator, perform function on previous operands. 
            //Save result into first used operand location and remove all other operands/operators used.
            {   
                case '+':
                    //Loading values
                    operand1 = equate(operand1, Post_num_Stack[i-2]);
                    operand2 = equate(operand2, Post_num_Stack[i-1]);
                    //calculating
                    for (int j = 0; j < operand2.Top; j++)//for every algebra power in operand2
                    {
                        if (find(operand1.powers, operand2.powers[j], operand1.Top) == -1)//allocate slot for algebra power in operand 1 if not present
                        {
                            operand1.Top++;
                            operand1.powers = (double*)realloc(operand1.powers, operand1.Top*sizeof(double));
                            operand1.values = (double*)realloc(operand1.values, operand1.Top*sizeof(double));
                            operand1.powers[operand1.Top-1] = operand2.powers[j];
                            operand1.values[operand1.Top-1] = 0;
                        }
                        operand1.values[find(operand1.powers, operand2.powers[j], operand1.Top)] += operand2.values[j];//Performing addition for each algebraic power
                    }
                    //removing all used operators/operands by shuffling all following terms forwards
                    i -= 1;
                    for (int j = i; j < Post_stack_Top - 2; j++)
                    {
                        Post_sign_Stack[j] = Post_sign_Stack[j + 2];
                        Post_num_Stack[j] = equate(Post_num_Stack[j],Post_num_Stack[j + 2]);
                    }
                    i -= 1;//pointing to resultant operand location
                    Post_stack_Top -= 2;//reduce postfix equation stack size 
                    Post_num_Stack[i] = equate(Post_num_Stack[i],operand1);
                    break;

                case '-':
                    //Loading values
                    operand1 = equate(operand1, Post_num_Stack[i-2]);
                    operand2 = equate(operand2, Post_num_Stack[i-1]);
                    //calculating
                    for (int j = 0; j < operand2.Top; j++)//for every algebra power in operand2
                    {
                        if (find(operand1.powers, operand2.powers[j], operand1.Top) == -1)//allocate slot for algebra power in operand 1 if not present
                        {
                            operand1.Top++;
                            operand1.powers = (double*)realloc(operand1.powers, operand1.Top*sizeof(double));
                            operand1.values = (double*)realloc(operand1.values, operand1.Top*sizeof(double));
                            operand1.powers[operand1.Top-1] = operand2.powers[j];
                            operand1.values[operand1.Top-1] = 0;
                        }
                        operand1.values[find(operand1.powers, operand2.powers[j], operand1.Top)] -= operand2.values[j];//Performing subtraction for each algebraic power
                    }
                    //removing all used operators/operands by shuffling all following terms forwards
                    i -= 1;
                    for (int j = i; j < Post_stack_Top - 2; j++)
                    {
                        Post_sign_Stack[j] = Post_sign_Stack[j + 2];
                        Post_num_Stack[j] = equate(Post_num_Stack[j],Post_num_Stack[j + 2]);
                    }
                    i -= 1;//pointing to first operand location
                    Post_stack_Top -= 2;//reduce postfix equation stack size 
                    Post_num_Stack[i] = equate(Post_num_Stack[i],operand1);//equate operand back to equation element
                    break;
                   
                case '*':
                    //Loading values
                    operand1 = equate(operand1, Post_num_Stack[i-2]);
                    operand2 = equate(operand2, Post_num_Stack[i-1]); 
                    //calculating
                    for (int j = 0; j < operand1.Top; j++)//for every algebra power in operand1
                    {
                        for (int k = 0; k < operand2.Top; k++)//for every algebra power in operand2
                        {   
                            if (find(operand3.powers,(operand1.powers[j]+operand2.powers[k]),operand3.Top) == -1)//allocate slot for new algebra power in operand 3 if needed
                            {
                                operand3.Top++;
                                operand3.powers = (double*)realloc(operand3.powers, operand3.Top*sizeof(double));
                                operand3.values = (double*)realloc(operand3.values, operand3.Top*sizeof(double));
                                operand3.powers[operand3.Top-1] = (operand1.powers[j]+operand2.powers[k]);
                                operand3.values[operand3.Top-1] = 0;
                            }
                            //performing multiplication on each pair of terms in operand 1 and 2, then saving the value into operand 3
                            operand3.values[find(operand3.powers,(operand1.powers[j]+operand2.powers[k]),operand3.Top)] += (operand1.values[j]*operand2.values[k]);
                        }
                    }
                    //removing all used operators/operands by shuffling all following terms forwards
                    i -= 1;
                    for (int j = i; j < Post_stack_Top - 2; j++)
                    {
                        Post_sign_Stack[j] = Post_sign_Stack[j + 2];
                        Post_num_Stack[j] = equate(Post_num_Stack[j],Post_num_Stack[j + 2]);
                    }
                    i -= 1;//pointing to first operand location
                    Post_stack_Top -= 2;//reduce postfix equation stack size 
                    Post_num_Stack[i] = equate(Post_num_Stack[i],operand3);//equate operand back to equation element
                    break;

                case '/':
                    //Loading values
                    operand1 = equate(operand1, Post_num_Stack[i-2]);
                    operand2 = equate(operand2, Post_num_Stack[i-1]);
                    //zero division check
                    int iszero = 1;
                    for (int k = 0; k < operand2.Top; k++)//for every algebra power in operand2
                    {
                        if (operand2.values[k] != 0)//if there is any non zero numeric value, operand 2 is nonzero
                        {
                            iszero = 0;
                        }
                    }
                    if (iszero == 1)//throw error and terminate if division by zero
                    {
                        printf("Error: Divide by zero!\n");
                        return;
                    }
                    for (int j = 0; j < operand1.Top; j++)//for every algebra power in operand1
                    {
                        for (int k = 0; k < operand2.Top; k++)//for every algebra power in operand2
                        {   
                            if (find(operand3.powers,(operand1.powers[j]-operand2.powers[k]),operand3.Top) == -1)//allocate slot for new algebra power in operand 3 if needed
                            {
                                operand3.Top++;
                                operand3.powers = (double*)realloc(operand3.powers, operand3.Top*sizeof(double));
                                operand3.values = (double*)realloc(operand3.values, operand3.Top*sizeof(double));
                                operand3.powers[operand3.Top-1] = (operand1.powers[j]-operand2.powers[k]);
                                operand3.values[operand3.Top-1] = 0;
                            }
                            //performing division on each pair of terms in operand 1 and 2, then saving the value into operand 3 (assuming operand 2 is a pure numeric value)
                            operand3.values[find(operand3.powers,(operand1.powers[j]-operand2.powers[k]),operand3.Top)] += (operand1.values[j]/operand2.values[k]);
                        }
                    }
                    //removing all used operators/operands by shuffling all following terms forwards
                    i -= 1;
                    for (int j = i; j < Post_stack_Top - 2; j++)
                    {
                        Post_sign_Stack[j] = Post_sign_Stack[j + 2];
                        Post_num_Stack[j] = equate(Post_num_Stack[j],Post_num_Stack[j + 2]);
                    }
                    i -= 1;//pointing to first operand location
                    Post_stack_Top -= 2;//reduce postfix equation stack size 
                    Post_num_Stack[i] = equate(Post_num_Stack[i],operand3);//equate operand back to equation element
                    break;
                    
                case '^':
                    //Loading values
                    //OPERAND 2 SHOULD NOT CONTAIN ALGEBRAIC TERMS
                    operand1 = equate(operand1, Post_num_Stack[i-2]);
                    operand2 = equate(operand2, Post_num_Stack[i-1]);
                    //Save operand 2 value into a local variable, the "exponent"
                    double exponent = operand2.values[find(operand2.powers,0,operand2.Top)];
                    //Make operand 2 a duplicate of operand1
                    operand2 = equate(operand2, operand1);

                    if (fmod(exponent, 1)!= 0 || exponent <= 0)//In case of a non positive integer power
                    {
                        for (int l = 0; l < operand1.Top; l++)
                        {
                            if (operand1.values[l] != 0 && operand1.powers[l] != 0)
                            {
                                printf("Error! Illegal input\n");//If algebra is detected in operand 2, throw error and terminate
                                exit(0);
                            }
                        }
                        //else it is a simple numeric^numeric situation. directly compute the value.
                        operand2.Top = 1;
                        operand2.powers[0] = 0;
                        operand2.values[0] = pow(operand1.values[0],exponent);
                    }
                    else
                    {
                        for (int l = 1; l < exponent; l++)//multiply the operands together, for exponent amount of times
                        {  
                            for (int j = 0; j < operand1.Top; j++)//for every algebra power in operand1
                            {
                                for (int k = 0; k < operand2.Top; k++)//for every algebra power in operand2
                                {   
                                    if (find(operand3.powers,(operand1.powers[j]+operand2.powers[k]),operand3.Top) == -1)//allocate slot for algebra power in operand 3 if not present
                                    {
                                        operand3.Top++;
                                        operand3.powers = (double*)realloc(operand3.powers, operand3.Top * sizeof(double));
                                        operand3.values = (double*)realloc(operand3.values, operand3.Top * sizeof(double));
                                        operand3.powers[operand3.Top-1] = (operand1.powers[j]+operand2.powers[k]);
                                        operand3.values[operand3.Top-1] = 0;
                                    }
                                    //performing multiplication on each pair of terms in operand 1 and 2, then saving the value into operand 3
                                    operand3.values[find(operand3.powers,(operand1.powers[j]+operand2.powers[k]), operand3.Top)] += operand1.values[j]*operand2.values[k];
                                }
                            }
                            operand2 = equate(operand2, operand3);
                            operand3 = reset(operand3);
                        }
                    }
                    //removing all used operators/operands by shuffling all following terms forwards
                    i -= 1;
                    for (int j = i; j < Post_stack_Top - 2; j++)
                    {
                        Post_sign_Stack[j] = Post_sign_Stack[j + 2];
                        Post_num_Stack[j] = equate(Post_num_Stack[j],Post_num_Stack[j + 2]);
                    }
                    i -= 1;//pointing to first operand location
                    Post_stack_Top -= 2;//reduce postfix equation stack size 
                    Post_num_Stack[i] = equate(Post_num_Stack[i],operand2);//equate operand back to equation element
                    break;

                case 'I':
                    operand1 = equate(operand1, Post_num_Stack[i-1]);
                    for (int j = 0; j < operand1.Top; j++)//for every algebra power in operand1
                    {
                        operand1.values[j] = -1 * operand1.values[j];//invert their numeric values
                    }
                    //removing all used operators/operands by shuffling all following terms forwards
                    for (int j = i; j < Post_stack_Top - 1; j++)
                    {
                        Post_sign_Stack[j] = Post_sign_Stack[j + 1];
                        Post_num_Stack[j] = equate(Post_num_Stack[j],Post_num_Stack[j + 1]);
                    }
                    i -= 1;//pointing to first operand location
                    Post_stack_Top -= 1;//reduce postfix equation stack size 
                    Post_num_Stack[i] = equate(Post_num_Stack[i],operand1);//equate operand back to equation element
                    break;
            }

            //Free unused arrays
            free(operand1.powers);
            free(operand1.values);
            free(operand2.powers);
            free(operand2.values);
            free(operand3.powers);
            free(operand3.values);
        }
        i++;
    }

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    //What formula looks like right now
    //Post_op_stack
    //[X]
    //Post_num_stack
    //[S]

    //An array of structs, only one term.  
    //The one term is the final answer, in the form of a mixed algebraic term struct.
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::    

    //Printing results in order of descending algebraic powers.
    double maxpow = 0;
    for (int j = 0; j < Post_num_Stack[0].Top; j++)//Finding the highest algebraic power
    {
        if (Post_num_Stack[0].powers[j] > maxpow)
        {
            maxpow = Post_num_Stack[0].powers[j];
        }
    }

    //For descending powers
    for (int j = maxpow; j >= 0; j--)
    {
        //If there exists a nonzero numeric value for that algebraic power
        if (find(Post_num_Stack[0].powers, j, Post_num_Stack[0].Top) != -1 && Post_num_Stack[0].values[find(Post_num_Stack[0].powers, j, Post_num_Stack[0].Top)] != 0)
        {
            if (j != maxpow && Post_num_Stack[0].values[find(Post_num_Stack[0].powers, j, Post_num_Stack[0].Top)] > 0)
            {
                printf("+");//Print plus if not the first term or not a negative numeric value
            }
            //Print numeric value
            printf("%.3f", Post_num_Stack[0].values[find(Post_num_Stack[0].powers, j, Post_num_Stack[0].Top)]);
            if (j != 0)//Print algebraic power
            {
                if (j == 1)
                {
                    printf("x");
                }
                else
                {
                    printf("x^%d.000", j);
                }
            }
        }
    }
    printf("\n");

    //Free arrays
    free(Post_num_Stack);
    free(Post_sign_Stack);
}

//=====================================================================================
void push(char* holding_String, int* holding_Top, char character)
{
    holding_String[*holding_Top] = character;
    *holding_Top += 1;
}
//=====================================================================================
char pop(char* holding_String, int* holding_Top)
{
    *holding_Top -= 1;
    return holding_String[*holding_Top];
}
//=====================================================================================
int find(double* list, double target, int length)
{
    for (int i = 0; i < length; i++)//for every list element
    {
        if (list[i] == target)//if list element matches target
        {
            return i;//return the index
        }
    }
    return -1;//return -1 if target not found in list
}
//=====================================================================================
struct Algebraic equate(struct Algebraic thing1, struct Algebraic thing2)//Replaces contents of thing1 with contents of thing2. ***Does not copy same variable address over***
{   
    //reallocate thing1 array sizes to match thing2 array sizes
    thing1.powers = (double*)realloc(thing1.powers, thing2.Top * sizeof(double));
    thing1.values = (double*)realloc(thing1.values, thing2.Top * sizeof(double));

    //Copy thing2 array contents to thing1
    for (int i = 0; i < thing2.Top; i++)
    {
        thing1.values[i] = thing2.values[i];
        thing1.powers[i] = thing2.powers[i];
    }
    thing1.Top = thing2.Top;

    return thing1;
}
//=====================================================================================
void dump(struct Algebraic thing1)//not really needed anymore, was for debug
{
    for (int i = 0; i < thing1.Top; i++)
    {
        printf("%f ", thing1.values[i]);
        printf("x^%f\n", thing1.powers[i]);
    }
}
//=====================================================================================
struct Algebraic reset(struct Algebraic thing1)
{
    //reset an algebraic struct to a defined 'empty' state
    thing1.Top = 1;
    thing1.values = (double*)calloc(1, sizeof(double));
    thing1.powers = (double*)calloc(1, sizeof(double));
    return thing1;
}
//=====================================================================================

