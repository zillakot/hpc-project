program tsp_genetic
	use global
	implicit none
	
	type(city), allocatable :: citylist(:), shortestroute(:)
	real(double), allocatable :: distances(:,:)
	real(double) :: shortestdistance
	
	call read_cities()
	write(*,*) "Found next cities from file:"
	call print_cities(citylist)
	write(*,*) "Randomize:"
	call random_population(citylist)
	call print_cities(citylist)
	call calculate_distances()
	
	write(*,*)
	write(*,*) "Number of paths analyzed:"!, count
	write(*,*)
	write(*,*) "Shortest distance:", shortestdistance, "km"
	write(*,*) "Shortest path::"

	call print_cities(shortestroute)
	stop

contains


subroutine read_cities()
	implicit none
	
	character(len = 20) :: d1 
	real(double)	:: d2,d3
	integer :: ios, st, i
	
	open(unit=10, file="input.txt", status="OLD", iostat=ios)
	if(ios/=0) then
			write(*,*) "File not found or error occurred"
			stop
	endif	
	write(*,*) "File opened succesfully"

	read(10,*) N
	allocate(citylist(N), shortestroute(N), distances(N,N), STAT=st)
	if(st /= 0) then
		write(*,*) "Memory allocation failed"
		stop
	endif

	do i=1,N
		read(10,*,IOSTAT = ios) d1, d2, d3
		if(ios /= 0) exit
	
		!read citylist names and 	coordinates and
		!transform coordinates to radians
		citylist(i)%name = d1
		citylist(i)%lat = d2*deg_to_rad
		citylist(i)%lon = d3*deg_to_rad
	
	end do
	close(10)
end subroutine read_cities 
 
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


subroutine print_cities(citylist)
	implicit none
	integer :: i
	type(city) :: citylist(:)

	do i=1,size(citylist)
		write(*,*) citylist(i)%name, citylist(i)%lat, citylist(i)%lon
	enddo

end subroutine print_cities

end program tsp_genetic