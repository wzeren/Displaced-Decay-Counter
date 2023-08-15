#!/usr/bin/env python
import os
import matplotlib as mpl        # Plotting module        
mpl.use('Agg')                  # Setting Matplotlib backend as non-interactive, necessary for use in Linus
import matplotlib.pyplot as plt # Plotting module


def detector_styles(detector):                      # Plotstyles for different detectors. This includes color in hexadecimal code, linestyle, and legend-label using LaTeX
    if detector == "ANUBIS0":                 #blue
        return "#005AC8","--", r'ANUBIS0, 3 ab$^{-1}$'
    if detector == "ANUBIS1":                 #blue 
        return "#005AC8","-", r'ANUBIS, 3 ab$^{-1}$'
    elif detector == "AL3X":                  #pink
        return "#00A0FA","-", r'AL3X, 250 fb$^{-1}$'
    elif detector == "CODEXB0":               #orange 
        return "#AA0A3C","--", r'CODEX-b0, 300 fb$^{-1}$'
    elif detector == "CODEXB1":               #orange
        return "#AA0A3C","-", r'CODEX-b, 300 fb$^{-1}$'
    elif detector == "FACET":                 #brown 
        return "#000000","-", r'FACET, 3 ab$^{-1}$'
    elif detector == "FASER":                 #green 
        return "#FA78FA","-", r'FASER, 150 fb$^{-1}$'
    elif detector == "FASER2":                #green 
        return "#ff0505","-", r'FASER2, 3 ab$^{-1}$'
    elif detector == "MAPP1":                 #red
        return "#0A9B4B", "-", r'MAPP1, 30 fb$^{-1}$'
    elif detector == "MAPP2":                 #red 
        return "#FF825F","-", r'MAPP2, 300 fb$^{-1}$'
    elif detector == "MATHUSLA0":             #yellow 
        return "#a39103","--", r'MATHUSLA0, 3 ab$^{-1}$'
    elif detector == "MATHUSLA1":             #yellow 
        return "#a39103","-", r'MATHUSLA, 3 ab$^{-1}$'
    elif detector == "MATHUSLA2":             #yellow 
        return "#a39103","-.", r'MATHUSLA2, 3 ab$^{-1}$'
    return "", "", ""

def load_data(benchmark,benchmarkscenario = ""):    # Reading data files. Returns a dictionary containing all the important information
    # Setup return dictionary
    data = dict()
    parent_directory = os.path.abspath(os.path.join(os.getcwd(), os.pardir))
    data["plotpath"] = os.path.join(parent_directory, "3_plots")
    data["datapath"] = os.path.join(parent_directory, "3_plots", "data_points"+benchmarkscenario)
    data["detectors"] = list()
    # Read data for every detector
    for filename in os.listdir(data["datapath"]):
        filepath = os.path.join(data["datapath"],filename)
        print("\tReading" +filepath)

        ctaus = list()
        yValues = list()
        if os.stat(filepath).st_size == 0:          # Skip it, if it is empty
                continue
        if filepath.split(".")[-1] != "dat":        # Skip it, if it is the wrong file
                continue
        with open(filepath, "r") as f:              # Read file, but skip detectors with very low sensitivity for certain benchmarks
            cur_det = filename.split(".")[0]
            if benchmark == "two" and cur_det == "FASER":
                continue
            if benchmark == "three" and (cur_det == "FASER" or cur_det == "FASER2"):
                continue  
            data["detectors"].append(cur_det)
            for line in f:
                tokens = line.split("\t")
                if len(tokens) == 1:                # If the split-line is empty, continue to next line.
                    continue
                ctaus.append(float(tokens[0]))
                yValues.append(float(tokens[1]))
            data[cur_det] = dict()
            data[cur_det]["ctau"] = ctaus
            data[cur_det]["yVal"] = yValues      
            data[cur_det]["color"], data[cur_det]["linestyle"], data[cur_det]["label"] = detector_styles(cur_det) 
    return data

def plot_make_up(ax,fontdict="",benchmark="",mstring =""):  # Creating a grid, labels, etc.
    if fontdict =="":
        fontdict = {'family': 'serif',
            'style': 'normal',
            'variant': 'normal',
            'weight': 'light',
            'size': 16}
        
    ax.set_xscale('log')                # Set log-scale of x-axis
    locmaj = mpl.ticker.LogLocator(base=10.0, subs=(0.1,1.0, ), numticks=100)
    ax.xaxis.set_major_locator(locmaj)
    locmin = mpl.ticker.LogLocator(base=10.0, subs=(0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1 ),numticks=100)
    ax.xaxis.set_minor_locator(locmin)
    ax.xaxis.set_minor_formatter(mpl.ticker.NullFormatter())
    
    ax.set_yscale('log')                # Set log-scale of y-axis
    locmaj = mpl.ticker.LogLocator(base=10.0, subs=(0.1,1.0, ), numticks=100)
    ax.yaxis.set_major_locator(locmaj)
    locmin = mpl.ticker.LogLocator(base=10.0, subs=(0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1 ),numticks=100)
    ax.yaxis.set_minor_locator(locmin)
    ax.yaxis.set_minor_formatter(mpl.ticker.NullFormatter())

    # Labels
    if benchmark == "one":
        plt.xlabel(r'c$\tau_{N}$ [m]')
        plt.ylabel(r'Br$(B^+/B^0 \to e^+/\bar{\nu}_e   N)$')
        plt.title(r'$m_N=1$~GeV')
        plt.axis([1E-2, 1E7, 1E-13, 1E-5]) 
        plt.legend(loc='lower right',prop={'size': 7})
    if benchmark == "two":
        plt.xlabel(r'c$\tau_{A_S}$ [m]')
        plt.ylabel(r'$\sigma (pp\rightarrow A\rightarrow A_S\,h_{\mathrm{SM}})$ [fb]')#($ggF\rightarrow A\rightarrow h+A_s$)
        if mstring =="A":
            plt.axhline((4.08/0.06), 1E-5, 1E10, color="black", linestyle="--",linewidth=1.75)
            plt.title(r'$M_{A}=410$~GeV,\,\,$M_{A_S}=70$~GeV')
        if mstring =="B":
            plt.axhline((1.26/0.06/0.66), 1E-5, 1E10, color="black", linestyle="--",linewidth=1.75)
            plt.title(r'$M_{A}=500$~GeV,\,\,$M_{A_S}=200$~GeV')
        plt.axis([1E-2, 1E7, 1E-1, 1E7]) 
        plt.legend(loc='lower right',prop={'size': 7})
    if benchmark == "three":
        plt.axhline(3.0, 1E-5, 1E10, color="black", linestyle="--",linewidth=2.25)
        plt.xlabel(r'c$\tau_{\tilde{\chi}^0_1}$ [m]')
        plt.ylabel(r'Signal Events')#($ggF\rightarrow A\rightarrow h+A_s$)
        #plt.ylabel(r'Br($\tilde{l}_L \rightarrow e^+\tilde{\chi}^0_1$)')
        plt.title(r'$m_{\tilde{e}_R}=500$~GeV,\,\,$m_{\tilde{\chi}^0_1}=1$~GeV')
        plt.axis([1E-4, 1E4, 1E-4, 1E4]) 
        plt.legend(loc='upper right',prop={'size': 7})

    # Plot a grid
    plt.grid(color='black',which='major',axis='x',alpha=0.5,linestyle='--',linewidth=0.5)
    #plt.grid(color='black',which='minor',axis='x',alpha=0.25,linestyle=':',linewidth=0.25) 
    plt.grid(color='black',which='major',axis='y',alpha=0.5,linestyle='--',linewidth=0.5)
    #plt.grid(color='black',which='minor',axis='y',alpha=0.25,linestyle=':',linewidth=0.25) 