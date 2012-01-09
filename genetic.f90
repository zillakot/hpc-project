subroutine random_population(citylist)
	use global
	implicit none
	
	integer i, chosenCity, seed
	type(city) :: citylist(N), usedCities(N)
	real(double) :: uniformrand
	
	i=1
	usedcities(:)%name="NULL"
	
	do while (i <= N) 

		chosenCity = uniformrand(seed)*N + 1
		write(*,*) "randomized number was ",chosenCity
 		!if this node was chosen already
		if(usedCities(chosenCity)%name /= "NULL") then

	    	!choose the next one of the list
	        do while(usedCities(chosenCity)%name /= "NULL")
	        	if(chosenCity + 1 <= N) then
	            	chosenCity=chosenCity+1
	            else 
	                chosenCity = 1
				endif
	        enddo
		endif

		usedCities(chosenCity)%name=citylist(i)%name
		usedCities(chosenCity)%lon=citylist(i)%lon
		usedCities(chosenCity)%lat=citylist(i)%lat
		
		i=i+1
	enddo
	
	citylist=usedCities

end subroutine random_population

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