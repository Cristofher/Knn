#make
FILE=plantilla

all:
	latex $(FILE).tex
	bibtex $(FILE).aux
	latex $(FILE).tex
	latex $(FILE).tex
	#dvipdfm -e -o $(FILE).pdf $(FILE).dvi   #desencaja las imagenes
	dvipdf  $(FILE).dvi $(FILE).pdf    #no genera embedded fonts


clean:
	rm *~ *.log *.aux *.bbl *.blg *.pdf *.dvi *.ps *.loa *.lof *.log  *.lot *.toc

