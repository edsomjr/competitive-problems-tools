% Modern format for competitive problems.
\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{cp_modern}
\LoadClass[12pt]{article}
 
\DeclareOption{portuguese}{\def\portuguese{yes}}
\ProcessOptions\relax

% Packages
\RequirePackage{fancyhdr}
\RequirePackage{tabularx,colortbl}
\RequirePackage{ifpdf}
\RequirePackage[T1]{fontenc}
\RequirePackage[utf8]{inputenc}
\RequirePackage[portuguese,english]{babel}
\RequirePackage{graphics}
\RequirePackage{graphicx}
\RequirePackage{amssymb, wrapfig}
\RequirePackage{xcolor,colortbl}
\RequirePackage{xcolor}
\RequirePackage{verbatim}
\RequirePackage[scaled=0.8]{helvet}
\RequirePackage{booktabs}
\RequirePackage{tikz}
\RequirePackage{enumerate}
\RequirePackage{amsmath}

\hyphenation{cons-tru-ção con-si-de-ra-da com-ple-xi-da-de in-di-vi-du-al-men-te}

\ifpdf
\RequirePackage[pdftex]{hyperref}
\else
\RequirePackage[hypertex]{hyperref}
\fi

% Setup
\pagenumbering{gobble}      % Remove page numbering

\oddsidemargin 0cm
\evensidemargin -2cm
\topmargin -1cm
\textwidth 17cm
\textheight 23cm

\DeclareGraphicsExtensions{png}
\pagestyle{fancy}

\hypersetup{
	letterpaper,
	colorlinks=true,
	linkcolor=blue,
	urlcolor=blue,
	pdfpagemode=none,
}

\renewcommand{\rmdefault}{ptm}

\setlength{\parskip}{0.25\baselineskip}
\setlength{\columnseprule}{3pt}
\setlength{\columnsep}{30pt}
\linethickness{0.02in} 

% Metadata
\def\yes{yes}
\newcommand{\translate}[2]{\ifx\portuguese\yes{#1}\else{#2}\fi}

% Problem

\newcommand{\header}[2]{\lhead{#1} \rhead{\the\year}}
\newcommand{\trailer}[2]{}

\newenvironment{problem}[4]{

	\begin{flushleft}
		{\LARGE \sffamily{{#1}\ \ \ \textbf{#2}}}
	\end{flushleft}
	\vspace{-0.3in}
	\begin{flushright}
        {\textit{\translate{Limite de tempo}{Timelimit}}: {#3}s}

        {\textit{\translate{Limite de memória}{Memory limit}}: {#4}MB}
	\end{flushright}
}

\newenvironment{probleminput}[1]{
	\begin{flushleft} 
		\textbf{\translate{Entrada}{Input}}
	\end{flushleft}
	\par

    \input{#1}
}

\newenvironment{problemoutput}[1]{
	\begin{flushleft} 
		\textbf{\translate{Saída}{Output}} \\
	\end{flushleft}
    \par 

    \input{#1}
}

\newenvironment{samples}[2]
{
	\begin{flushleft} 
		\textbf{\translate{Exemplos}{Samples}} \\
	\end{flushleft}
    \par 
}
{
}

\newcommand{\iosample}[4]
{
    \begin{center}
	\begin{tabular}{p{#1}p{#2}}
	\hline
	\rowcolor{black} \textbf{\textcolor{white}{\translate{Entrada}{Input}}} & \textbf{\textcolor{white}{\translate{Saída}{Output}}} \\
    \hline
    \begin{minipage}[t]{#1}{ \verbatiminput{#3} }
    \end{minipage} & 
    \begin{minipage}[t]{#2}{ \verbatiminput{#4} }
    \end{minipage} \\
	\bottomrule
    \end{tabular}	
    \end{center}
}

\newenvironment{problemnotes}[1]{

	\begin{flushleft} 
		\textbf{\translate{Notas}{Notes}} \\
	\end{flushleft}
    \par 

    \input{#1}
}
{
}

\newenvironment{customnotes}[3]{
	\bigskip
	\begin{flushleft}
		\textbf{\translate{#1}{#2}} \\
	\end{flushleft}

    \input{#3}
}
{
}
