print("Loading Libraries")
import sys,os                   # Basic functions
import matplotlib as mpl        # Plotting module        
mpl.use('Agg')                  # Setting Matplotlib backend as non-interactive, necessary for use in Linus
import matplotlib.pyplot as plt # Plotting module
import functions as func

#######################################################################################
# Reading Data
print("Loading Input")
data_dict = func.load_data("one")
#######################################################################################
# Setting regarding the typewriting of axis and label
plt.rc("text", usetex=True)
font = {'family': 'serif',
        'style': 'normal',
        'variant': 'normal',
        'weight': 'light',
        'size': 16}
plt.rc('font', **font)
mpl.rc('font', **font)

# Setting plots
fig = plt.figure(num=None, figsize=(5, 4), dpi=300, facecolor='w', edgecolor='k')
ax = fig.add_subplot(1, 1, 1)
#######################################################################################
# Plotting each detector
for det in data_dict["detectors"]:
    print("Plotting "+det)
    plt.loglog(data_dict[det]["ctau"], data_dict[det]["yVal"], color=data_dict[det]["color"], linestyle = data_dict[det]["linestyle"], linewidth=1, label=data_dict[det]["label"])
#######################################################################################
# Creating a grid, labels, etc., and saving plot
func.plot_make_up(ax,fontdict = font,benchmark = "one",mstring ="")
os.system('if not exist "'+data_dict["plotpath"]+'" mkdir '+data_dict["plotpath"])
savepath = os.path.join(data_dict["plotpath"],"benchmark_one.pdf")
plt.savefig(savepath,format="pdf",bbox_inches='tight',dpi=200)