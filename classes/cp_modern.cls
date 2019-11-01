% Document class used to format TEP problems.

\ProvidesClass{cp_modern}

\LoadClass[12pt]{article}

% Packages
\usepackage{mmap}
\usepackage[T1]{fontenc}
\usepackage[brazil]{babel}
\usepackage[utf8]{inputenc}
\usepackage{graphicx}
\usepackage[top=1in,bottom=1in,left=1in,right=1in]{geometry}
\usepackage[scaled=0.8]{helvet}
\usepackage{enumerate}
\usepackage{multicol}
\usepackage[table]{xcolor}
\usepackage{array}
\usepackage{textcomp}
\usepackage{xtab}
\usepackage{booktabs}
\usepackage{listings}
\usepackage{multicol}
\usepackage{amsmath}
\usepackage{amssymb}
\usepackage{tikz}
\usepackage{tabularx}
\usepackage{fancyhdr}
\usepackage{mathtools}

% Novos comandos
\renewcommand{\rmdefault}{ptm}
\renewcommand{\lstlistingname}{Código}
\renewcommand{\lstlistlistingname}{Lista de Códigos}

\newcommand{\sen}{\operatorname{sen}}
\newcommand{\tg}{\operatorname{tg}}
\newcommand{\cotg}{\operatorname{cotg}}

\newcommand{\arcsen}{\operatorname{arcsen}}
\newcommand{\arctg}{\operatorname{arctg}}

\newcommand{\senh}{\operatorname{senh}}
\newcommand{\tgh}{\operatorname{tgh}}

\newcommand{\arcsenh}{\operatorname{arcsenh}}
\newcommand{\arctgh}{\operatorname{arctgh}}

\newcommand{\noun}[1]{\textsc{#1}}

\newcommand{\gabVF}[1]{\texttt{( {\bf #1} )}}
\newcommand{\solucao}[1]{\textit{\textbf{Solução:} #1}}
\newcommand{\demonstracao}[1]{\textit{\textbf{Demonstração:} #1} 
	\begin{flushright} \ensuremath{\square} \end{flushright}} 
\newcommand{\gabM}{\item[({\bf x})] \stepcounter{enumii}}

\newcount\colveccount
\newcommand*\colvec[1]{
        \global\colveccount#1
        \begin{bmatrix}
        \colvecnext
}
\def\colvecnext#1{
        #1
        \global\advance\colveccount-1
        \ifnum\colveccount>0
                \\
                \expandafter\colvecnext
        \else
                \end{bmatrix}
        \fi
}

\newcommand{\eulerian}[2]{\genfrac{<}{>}{0pt}{}{#1}{#2}}

\newcommand{\fancyfootnotetext}[2]{%
  \fancypagestyle{dingens}{%
    \fancyfoot[LO,RE]{\parbox{16cm}{\footnotemark[#1]\footnotesize #2}}%
  }%
  \thispagestyle{dingens}%
}

\renewcommand{\footrulewidth}{\headrulewidth}
\def\imod#1{\allowbreak\mkern5mu({\operator@font mod}\,\,#1)}

% Configurações
\setlength{\parskip}{0.25\baselineskip}
\setlength{\columnseprule}{3pt}
\setlength{\columnsep}{30pt}
\linethickness{0.02in} 

\lstset{numbers=left,
stepnumber=1,
firstnumber=1,
numberstyle=\tiny,
extendedchars=true,
breaklines=true,
frame=tb,
tabsize=2,
basicstyle=\footnotesize,
stringstyle=\ttfamily,
showstringspaces=false
}

% Comandos específicos para questões

\newcommand{\name}[1]{\def\imprimirname{#1}}

\newcommand{\area}[1]{\def\imprimirarea{#1}}
\def\FundamentosDeProgramacao{1}
	\def\IO{1}
	\def\Arrays{2}

\def\Strings{2}
	\def\Parser{1}
	\def\Match{2}

\def\EstruturasDeDados{3}
	\def\PilhasEFilas{1}
	\def\Vetores{2}
	\def\ListasCirculares{3}

\def\Matematica{4}
	\def\AlgebraLinear{1}
	\def\AnaliseCombinatoria{2}
	\def\RecursaoERecorrencias{3}
	\def\TeoriaDosNumeros{4}
	\def\Calculo{5}
	\def\Aritmetica{6}

\def\AlgoritmosEParadigmas{5}
	\def\Backtracking{1}
	\def\Ordenacao{3}

\def\Grafos{6}
\def\GeometriaComputacional{7}
\def\Jogos{8}
\def\Outros{9}

\newcommand{\subarea}[1]{\def\imprimirsubarea{#1}}

\newcommand{\id}[1]{\def\imprimirid{\ifnum#1<100 0\fi \ifnum#1<10 0\fi\number#1}}

\newcommand{\timeout}[1]{\def\imprimirtimeout{#1}}

\newcommand{\level}[1]{\def\imprimirlevel{#1}}
\def\Beginner{1}
\def\ICC{2}
\def\ComputacaoBasica{3}
\def\EDA{4}
\def\Jogos{5}
\def\PPC{6}
\def\Maratona{7}
\def\Final{8}
\def\Pesquisa{9}

\newcommand{\autor}[1]{\def\imprimirautor{#1}}
\newcommand{\email}[1]{\def\imprimiremail{#1}}
\newcommand{\serie}[1]{\def\imprimirserie{#1}}
\newcommand{\numero}[1]{\def\imprimirnumero{#1}}
\newcommand{\timelimit}[1]{\def\imprimirtimelimit{#1}}

\newcommand{\autorname}{\imprimirautor}
\newcommand{\autoremail}{\imprimiremail}
\newcommand{\problemserie}{\imprimirserie}
\newcommand{\problemnumber}{\imprimirnumero}

\newcommand{\disclaimer}{\textit{Este problema foi elaborado 
para ensino e docência. Quaisquer coincidências com problemas já existentes
favor entrar em contato (\autoremail) para que as devidas providências
sejam tomadas.}}

\pagestyle{fancy}
\lhead{\small \textbf{\problemserie}}
\rhead{\small {\autorname}}
\lfoot{\footnotesize Contato: \autoremail}
\rfoot{\footnotesize \the\year}
\cfoot{}

\newcommand{\TL}{Limite de Tempo}

\newenvironment{question} {
	\begin{flushleft}
		{\LARGE \sffamily{{\problemnumber}\ \ \ \textbf{\imprimirname}}}
	\end{flushleft}
	\vspace{-0.5in}
	\begin{flushright}
        {\textit{\TL:} \imprimirtimelimit s}
	\end{flushright}
}
{
	\vspace*{0.2in}

	\noindent{\small \disclaimer}
}

\newcommand{\entradas}{Entrada}
\newcommand{\saidas}{Saída}
\newcommand{\exemplosentradas}{Exemplos de entradas}
\newcommand{\exemplossaidas}{Exemplos de saídas}

\newenvironment{inputs}
{
	\begin{flushleft} 
		\textbf{\entradas}
	\end{flushleft}
	\par
}
{
}

\newenvironment{outputs} 
{
	\begin{flushleft} 
		\textbf{\saidas}
	\end{flushleft}
	\par
}
{
}

\newcommand{\inputrowsize}{6cm}
\newcommand{\outputrowsize}{8cm}

\newenvironment{iosamples} 
{
\begin{center}
\begin{tabular}{p{\inputrowsize}p{\outputrowsize}}
\toprule
\textbf{\exemplosentradas} & \textbf{\exemplossaidas} \\
\toprule 
}
{
\bottomrule
\end{tabular}
\end{center}
}

\newcounter{samplenum}
\setcounter{samplenum}{1}

\newcommand{\iosample}[2]{\ifnum\thesamplenum<1 \rowcolor[gray]{0.9} \setcounter{samplenum}{1}\else \setcounter{samplenum}{0}\fi\texttt{#1} & \texttt{#2} \\ }

\DeclarePairedDelimiter{\ceil}{\lceil}{\rceil}
\DeclarePairedDelimiter{\floor}{\lfloor}{\rfloor}
