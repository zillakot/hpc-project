subroutine create_population(population)
	use global
	implicit none
	
	integer :: i, j, chosenCity, seed
	integer :: usedCities(N)
	integer :: population(N,numberOfPopulation)
	real(double) :: uniformrand
	
	population=0
	do j=1,numberOfPopulation
		i=1
		do while (i <= N) 

			chosenCity = uniformrand(seed)*N + 1
 			!if this node was chosen already
			if(population(chosenCity,j) /= 0) then

	    		!choose the next one of the list
	        	do while(population(chosenCity,j) /= 0)
	        		if(chosenCity + 1 <= N) then
	            		chosenCity=chosenCity+1
	            	else 
	                	chosenCity = 1
					endif
	        	enddo
			endif

			population(chosenCity,j)=i
			write(*,*) population(j,chosenCity), chosenCity
			i=i+1
		enddo
	enddo

end subroutine create_population

subroutine sort_population(population,distances)
	use global
	implicit none
	integer :: population(N,numberOfPopulation)
	real :: fitnessArray(numberOfPopulation)
	real :: distances(N,N)
	
	call get_fitness(population,distances,fitnessArray)
	
	
end subroutine sort_population

subroutine get_fitness(population,distances,fitnessArray)
	use global
	implicit none
	integer i,j
	integer :: population(N,numberOfPopulation)
	real :: distances(N,N)
	real :: s
	real :: fitnessArray(numberOfPopulation)
	fitnessArray=0
	
	do i=1,numberOfPopulation	
		s=0
		do j=1,N-1
			s=s+distances(population(j,i),population(j+1,i))
		enddo
		    fitnessArray(i)=s+distances(population(N,i),population(1,i))
	enddo
	write(*,*) fitnessArray
	
end subroutine get_fitness

subroutine mate_population(population)
	use global
	implicit none
	integer :: population(N,numberOfPopulation)
end subroutine mate_population

subroutine mutate_population(population)
	use global
	implicit none
	integer :: population(N,numberOfPopulation)
end subroutine mutate_population

function uniformrand(seed)
  use global
  implicit none

  ! -----------------------------------------------------------
  ! Park-Miller "minimal" Random number generator
  ! uniform distribution ]0,1[ . See Numerical Recipes ch. 7.0
  ! -----------------------------------------------------------

  real(double) :: uniformrand
  integer :: seed
  integer :: IA,IM,IQ,IR,MASK
  real(double) :: AM
  integer :: k
  
  parameter (IA=16807,IM=2147483647,AM=1.0/IM,IQ=127773,IR=2836,MASK=123459876)

  seed=ieor(seed,MASK)
  k=seed/IQ
  seed=IA*(seed-k*IQ)-IR*k
  if (seed < 0) seed=seed+IM
  uniformrand=AM*seed
  seed=ieor(seed,MASK)
  
  return  
end function uniformrand