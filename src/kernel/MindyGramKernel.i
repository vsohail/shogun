#ifdef HAVE_MINDY

%{
 #include "kernel/MindyGramKernel.h" 
%}

%rename (MindyGramKernel) CMindyGramKernel;

%include "kernel/MindyGramKernel.h" 
#endif