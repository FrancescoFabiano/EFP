import sys
import pandas as pd
from pathlib import Path


def loopPrintLaTeX(line,narg,tableFile):
    for k in range(narg):
        print(line,file=tableFile,end="")

def loopPrintLaTeXArray(line_before,endline,endSequence,array,tableFile):
    for elem in array[:-1]:
        print(line_before + str(elem),file=tableFile,end=endline)
    print(line_before + str(array[-1]),file=tableFile,end=endSequence)

def loopPrintLaTeXArrayTime(line_before,endline,endSequence,array,tableFile):
    decimals=3
    countprint = 0
    for elem in array[:-1]:
        countprint+=1
        if countprint > 2 and countprint < 8:
            time = elem
            if elem != "TO":
                elem = round(float(elem),decimals)
        print(line_before + str(elem),file=tableFile,end=endline)
    print(line_before + str(array[-1]),file=tableFile,end=endSequence)

if __name__ == '__main__':


    filename='out/Heur-tests/graphres.csv'
    # Read csv / tab-delimited in this example
    df = pd.read_csv(filename, sep=',')
    
    df = df[['Domain', 'Instance', 'Time-BFS', 'Time-LENGTH_PG', 'Time-SUM_PG', 'Time-CLASSIC_PG', 'Time-SUBGOALS', 'Nodes-BFS', 'Nodes-LENGTH_PG', 'Nodes-SUM_PG', 'Nodes-CLASSIC_PG', 'Nodes-SUBGOALS', 'Length-BFS', 'Length-LENGTH_PG', 'Length-SUM_PG', 'Length-CLASSIC_PG', 'Length-SUBGOALS']]

    # Write csv
    df.to_csv(filename, sep=',',index=False)

    #Create LaTeX Table
    table_path="out/Heur-tests/Table/"
    Path(table_path).mkdir(parents=True,exist_ok=True)

    heur_number = 5
    field_number = 3
    with open(table_path+"table.tex", "w") as tableFile:
        #Preamble
        print('\\documentclass[12pt,a4paper]{standalone}\n\\usepackage{hhline}\n\\usepackage{tabularx}\n\\begin{document}',file=tableFile)
        
        #Tabular declaration
        print('\t\\begin{tabular}{||l|l|',file=tableFile,end="")
        for k in range (field_number):
            loopPrintLaTeX('|c',heur_number,tableFile)
            print("|",file=tableFile,end="")
        print('|}',file=tableFile)

        numpyHead = (df.columns.to_numpy())
        numpyDF = df.to_numpy()
        #Approaches row
        print('\t\t\\hhline{|t:==:tt:',file=tableFile,end="")
        for k in range (field_number):
            loopPrintLaTeX('=',heur_number,tableFile)
            if k+1 < field_number:
                print(":tt:",file=tableFile,end="")
        print(':t|}',file=tableFile)

        loopPrintLaTeXArray("\t\t"," & ","\\\\\n",numpyHead,tableFile)
        domain="Domain"
        printedTime=False
        for i in numpyDF:
            if domain != i[0]:
                domain=i[0]
                print('\t\t\\hhline{|:==::',file=tableFile,end="")
                for k in range (field_number):
                    loopPrintLaTeX('=',heur_number,tableFile)
                    if k+1 < field_number:
                        print("::",file=tableFile,end="")
                print(':|}',file=tableFile)
            else:
                print('\t\t\\hhline{||-|-||',file=tableFile,end="")
                for k in range (field_number):
                    loopPrintLaTeX('-|',heur_number,tableFile)
                    if k+1 < field_number:
                        print("|",file=tableFile,end="")
                print('|}',file=tableFile)
            loopPrintLaTeXArrayTime("\t\t"," & ","\\\\\n",i,tableFile)


        print('\t\t\\hhline{|b:==:bb:',file=tableFile,end="")
        for k in range (field_number):
            loopPrintLaTeX('=',heur_number,tableFile)
            if k+1 < field_number:
                print(":bb:",file=tableFile,end="")
        print(':b|}',file=tableFile)



        print('\t\\end{tabular}\n\\end{document}',file=tableFile)


    with open(table_path+"table.tex", 'r') as file:
        data = file.read()
        
        data = data.replace("Time", "T")
        data = data.replace("Nodes", "N")
        data = data.replace("Length", "L")

        data = data.replace("LENGTH_PG", "L_PG")
        data = data.replace("SUM_PG", "S_PG")
        data = data.replace("CLASSIC_PG", "C_PG")
        data = data.replace("SUBGOALS", "SUB")

        data = data.replace("_", "\\_")

    with open(table_path+"table.tex", 'w') as file:
        file.write(data)