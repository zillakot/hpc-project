F90= gfortran 
# F90=ifort -O

MODULES= global.o
OBJECTS= tsp_genetic.o genetic.o
TARGET= tsp_genetic


tsp_genetic:		$(OBJECTS) $(MODULES)
			$(F90) -o $(TARGET) $(OBJECTS) $(MODULES)


tsp_genetic.o:		tsp_genetic.f90 $(MODULES)
			$(F90) -c tsp_genetic.f90

genetic.o:		genetic.f90 $(MODULES)
			$(F90) -c genetic.f90

global.o:	global.f90 
		$(F90) -c global.f90


clean:
			rm -f *.o *.mod $(TARGET)