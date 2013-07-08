import matplotlib.pyplot as plt
from matplotlib.font_manager import FontProperties
import numpy as np
import os

# plot color 
#plt.gca().set_color_cycle(['red', 'green', 'blue', 'yellow'])
#num_plots = 4
#colormap = plt.cm.gist_ncar
#plt.gca().set_color_cycle([colormap(i) for i in np.linspace(0, 0.9, num_plots)])

# font sizes 
fontP = FontProperties()
fontP.set_size('small')

# run OpenMOC


geometries = ['geometry_c5g7.xml']
materials = ['material_c5g7.xml']

#geometries = ['geometry_LRA.xml', 'geometry_UO2.xml', 'geometry_c5g7_wo_refl.xml']
#materials = ['material_LRA.xml', 'material_c5g7.xml','material_c5g7.xml' ]

#geometries=['geometry_pin52.xml']#, 'geometry_pin51.xml', 'geometry_pin52.xml']
#materials=['material_simple.xml','material_simple.xml','material_simple.xml']

ts = [0.1]
na = [16]
fc = [1e-4]

for i, geometry in enumerate(geometries):
    for spacing in ts:
        for angle in na:
            os.system('../bin/openmoc'
                      + ' -m ../xml-sample/Cmfd/' + materials[i]
                      + ' -g ../xml-sample/Cmfd/' + geometry 
                      + ' -na ' + str(angle) 
                      + ' -ts ' + str(spacing) 
                      + ' -fc ' + str(fc[0]) 
					  + ' -wc -df 1.0')
            os.system('../bin/openmoc'
                      + ' -m ../xml-sample/Cmfd/' + materials[i]
                      + ' -g ../xml-sample/Cmfd/' + geometry 
                      + ' -na ' + str(angle) 
                      + ' -ts ' + str(spacing) 
                      + ' -fc ' + str(fc[0])
					  + ' -wc -df 0.7')
            os.system('../bin/openmoc'
                      + ' -m ../xml-sample/Cmfd/' + materials[i]
                      + ' -g ../xml-sample/Cmfd/' + geometry 
                      + ' -na ' + str(angle) 
                      + ' -ts ' + str(spacing) 
                      + ' -fc ' + str(fc[0])
					  + ' -wl1 -df 1.0')
            os.system('../bin/openmoc'
                      + ' -m ../xml-sample/Cmfd/' + materials[i]
                      + ' -g ../xml-sample/Cmfd/' + geometry 
                      + ' -na ' + str(angle) 
                      + ' -ts ' + str(spacing) 
                      + ' -fc ' + str(fc[0])
					  + ' -wl -df 1.0')

    # list of l2_norm files
    l2_norm_files = []

    # make figure objects
    #fig_l2_norm = plt.figure(1)
    #fig_keff = plt.figure(2)

    # get all l2_norm files in directory
    for file in os.listdir("."):
        if file.startswith("l2_norm") and file.endswith(".txt"):
            l2_norm_files.append(file)

    # parse output files
    for file in l2_norm_files:
        logfile = open(file, "r").readlines()
        os.rename(file, geometry[:-4] + '_' + file)
        
        for c_num, c in enumerate(file):
            if c == '_':
                method = file[-8:-4]
                break

        for c_num, c in enumerate(file):
            if c == '_':
                damp = file[-12:-9]
                break

        # find number of lines in file
        for num_lines, l in enumerate(logfile):
            pass

        # create numpy arras
        iteration = np.zeros(num_lines)
        cell_l2   = np.zeros(num_lines)
        fsr_linf  = np.zeros(num_lines)
        fsr_l2    = np.zeros(num_lines)
        num       = np.zeros(num_lines)

        for i, line in enumerate(logfile):
            if i is not 0:
                iteration[i-1] = line.split()[0]
                cell_l2[i-1]   = line.split()[1]
                fsr_linf[i-1]  = line.split()[2]
                fsr_l2[i-1]      = line.split()[3]
                num[i-1]       = line.split()[5]

		# plot l2 norm
        plt.figure(1)
        fit_coeff = np.polyfit(iteration, np.log(cell_l2), 1)
        fit_vals  = np.polyval(fit_coeff, iteration)
        plt.semilogy(iteration, np.exp(fit_vals))
        plt.semilogy(iteration, cell_l2, '+', label = ("%s damp %s" %
		(method, damp)))
        plt.xlim(0, iteration[num_lines-1] + 1)
        plt.legend(loc='upper center', ncol=3, prop = fontP, shadow=True, 
				   bbox_to_anchor=(0.5,-0.1), 				
				   fancybox=True)
		
        plt.figure(2)
        #fit_coeff = np.polyfit(iteration, np.log(fsr_linf), 1)
        #fit_vals  = np.polyval(fit_coeff, iteration)
        #plt.semilogy(iteration, np.exp(fit_vals))
        plt.semilogy(iteration, fsr_linf, '+', label=("%s damp %s" %
		(method, damp)))
        plt.xlim(0, iteration[num_lines-1] + 1)
        plt.legend(loc='upper center', ncol=3, prop = fontP, shadow=True, 
				   bbox_to_anchor=(0.5,-0.1), 				
				   fancybox=True)
		
        plt.figure(3)
        fit_coeff = np.polyfit(iteration, np.log(fsr_l2), 1)
        fit_vals  = np.polyval(fit_coeff, iteration)
        plt.semilogy(iteration, np.exp(fit_vals))
        plt.semilogy(iteration, fsr_l2, '+', label= ("%s damp %s" %
		(method, damp)))
        plt.xlim(0, iteration[num_lines-1] + 1)
        plt.legend(loc='upper center', ncol=3, prop = fontP, shadow=True, 
				   bbox_to_anchor=(0.5,-0.1), 				
				   fancybox=True)		
        plt.figure(4)
        plt.plot(iteration, num, '+', label = ("%s damp %s" %
		(method, damp)))
        plt.xlim(0, iteration[num_lines-1] + 1)
        plt.legend(loc='upper center', ncol=3, prop = fontP, shadow=True, 
				   bbox_to_anchor=(0.5,-0.1), 				
				   fancybox=True)	
    plt.figure(1)
    plt.xlabel('# MOC iteration')
    plt.ylabel('Mesh Cell L2 Norm on Fission Source')
    plt.title('Geometry: %s,'%(geometry[9:-4]) + ' spacing: %s,'%str(ts[0]) 
    		  + ' #angles: %s'%str(na[0]))
    plt.savefig(geometry[9:-4] + '_cell_l2.png', bbox_inches='tight')

    plt.figure(2)
    plt.xlabel('# MOC iteration')
    plt.ylabel('FSR L-infinity Norm on Fission Source')
    plt.title('Geometry: %s,'%(geometry[9:-4]) + ' spacing: %s,'%str(ts[0]) 
			  + ' #angles: %s'%str(na[0]))
    plt.savefig(geometry[9:-4] + '_fsr_linf.png', bbox_inches='tight')

    plt.figure(3)
    plt.xlabel('# MOC iteration')
    plt.ylabel('FSR L2 Norm on Fission Source')
    plt.title('Geometry: %s,'%(geometry[9:-4]) + ' spacing: %s,'%str(ts[0]) 
			  + ' #angles: %s'%str(na[0]))
    plt.savefig(geometry[9:-4] + '_fsr_l2.png', bbox_inches='tight')

    plt.figure(4)
    plt.xlabel('# MOC iteration')
    plt.ylabel('# Acceleration Iterations Taken at Each MOC Iteration')
    plt.title('Geometry: %s,'%(geometry[9:-4]) + ' spacing: %s,'%str(ts[0]) 
			  + ' #angles: %s'%str(na[0]))
    plt.savefig(geometry[9:-4] + '_num_acc.png', bbox_inches='tight')
