subroutine read_lines()
	use global
	implicit none
	integer :: ios
	
	open(unit=10, file="input.txt", status="OLD", iostat=ios)
	if(ios/=0) stop "File not found or error occurred"
	
	read(10,*) N

end subroutine read_lines

subroutine read_cities(citylist)
	use global
	implicit none
	
	type(city) :: citylist(*)
	character(len = 20) :: d1
	real(double)	:: d2,d3
	integer :: ios, i, st
	
	do i=1,N
		read(10,*,IOSTAT = ios) d1, d2, d3
		if(ios /= 0) exit
		write(*,*) d1,d2,d3
	
		!read citylist names and 	coordinates and
		!transform coordinates to radians
		citylist(i)%name = d1
		citylist(i)%lat = d2*deg_to_rad
		citylist(i)%lon = d3*deg_to_rad
	
	end do
	close(10)
end subroutine read_cities

subroutine print_cities(citylist)
	use global
	implicit none
	integer :: i
	type(city) :: citylist(:)

	do i=1,size(citylist)
		write(*,*) citylist(i)%name, citylist(i)%lat, citylist(i)%lon
	enddo

end subroutine print_cities