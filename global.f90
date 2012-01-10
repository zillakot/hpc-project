module global
	implicit none
	
	integer, parameter :: double = selected_real_kind(10,40)
	integer, parameter :: numberOfPopulation=100
	integer, parameter :: mutationRate=0.1
	integer, parameter :: maxGenerations=2
	integer :: N
	real(double), parameter :: pi = 3.1415926535
	real(double), parameter :: deg_to_rad = pi/180.0
	
	type city
	character(len = 20) :: name
	real :: lat,lon
	end type city
end module global