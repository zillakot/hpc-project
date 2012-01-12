program tsp_genetic
	use global
	implicit none
	
	type(city), allocatable :: citylist(:)
	real(double), allocatable :: distances(:,:)
	integer, allocatable :: population(:,:)
	!real(double) : shortestdistance
	integer :: numGenerations, st
	
	call read_lines()
	
	allocate(citylist(N),distances(N,N), population(N,numberOfPopulation), STAT=st)
	if(st /= 0) stop "Memory allocation failed"
	
	call read_cities(citylist)
	
	write(*,*) "Found next cities from file:"
	call print_cities(citylist)
	
	call calculate_distances()
	write(*,*) distances
	call create_population(citylist, population)
	
	do while(numGenerations < maxGenerations)

		numGenerations=numGenerations+1;

		call sort_population(population,distances)
        call mate_population(population)
		call mutate_population(population)

		if(modulo(numGenerations,1000) == 0) then
			write(*,*)"* Generations... ", numGenerations
		endif

	enddo
	
	
	call print_cities(citylist)
	
	write(*,*)
	write(*,*) "Number of paths analyzed:"!, count
	write(*,*)
	write(*,*) "Shortest distance:",  "km"
	write(*,*) "Shortest path::"

	!call print_cities(shortestroute)
	stop

contains

subroutine calculate_distances()
	implicit none

	integer :: st, i, j
	real :: r=6371.0
	
	do i=1,size(citylist)
		 do j=1,size(citylist)
			 if (i==j) distances(i,j)=0
			 if (i/=j) then
			distances(i,j)=r*Acos(Cos(citylist(i)%lat)*Cos(citylist(j)%lat)&
			&*Cos(citylist(i)%lon-citylist(j)%lon)+Sin(citylist(i)%lat)*Sin(citylist(j)%lat))
		 endif
		 end do
	end do
	!write(*,*) distances
end subroutine calculate_distances

end program tsp_genetic