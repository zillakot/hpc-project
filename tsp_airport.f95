program airportappro
  implicit none

  !Define new datatype for targets
  type map_target
    character(len = 20) :: name
    real :: lon,lat
  end type map_target
  
  !Define variables
  integer :: N, count, st
  character(len = 20) :: filename
  type(map_target), allocatable :: airport(:), shortestroute(:)
  integer, allocatable :: stack(:)
  real, allocatable :: distances(:,:)
  real :: shortestdistance = 100000
  real :: pi=3.141592

  write(*,*) ""
  read(*,*) filename
  
  call read_file(filename,airport,N)
  call calculate_distances(airport,N,distances)
  
  count=0
  stack(:)=1
  call shortest_path(airport,stack,1,count,N,distances,shortestdistance,shortestroute)
  write(*,*) count, shortestdistance

  call print_path(shortestroute)
  stop
contains


subroutine read_file(filename, airport, N)
  implicit none
  
  type(map_target), allocatable :: airport(:)
  character(len = 20) :: filename
  character(len = 20) :: d1 
  character(len = 1)  :: d5, d9
  

  real    :: d2,d3,d4,d6,d7,d8 
  integer :: N, ios, st, i
  
  open(unit=10, file=filename, status="OLD", iostat=ios)
  if(ios/=0) then
	write(*,*) "File not found or error occurred"
	stop
  endif  
  write(*,*) "File opened succesfully"

  read(10,*) N
  allocate(airport(N), shortestroute(N), stack(N), distances(N,N), STAT=st)
  if(st /= 0) then
	write(*,*) "Memory allocation failed"
    stop
  endif

  do i=1,N
    read(10,*,IOSTAT = ios) d1, d2, d3, d4, d5, d6, d7, d8, d9
    if(ios /= 0) exit
	
	airport(i)%name = d1
	airport(i)%lat = (d2 + d3/60.0 + d4/3600.0)*pi/180.0
	airport(i)%lon = (d6 + d7/60.0 + d8/3600.0)*pi/180.0
	if(trim(d5)=='S') airport(i)%lat=-airport(i)%lat
	if(trim(d9)=='W') airport(i)%lon=-airport(i)%lon
	write(*,*) airport(i)%name, airport(i)%lat*180.0/pi, airport(i)%lon*180.0/pi
  end do
  close(10)
  write(*,*)
end subroutine read_file 
 
 
subroutine calculate_distances(airport,N,distances)
  implicit none

  type(map_target) :: airport(:)
  integer     :: N, st, i, j
  real, allocatable :: distances(:,:)
  real :: r=6371.0

  
  do i=1,N
     do j=1,N
       if (i==j) distances(i,j)=0
       if (i/=j) then
			distances(i,j)=r*Acos(Cos(airport(i)%lat)*Cos(airport(j)%lat)&
			&*Cos(airport(i)%lon-airport(j)%lon)+Sin(airport(i)%lat)*Sin(airport(j)%lat))
	   endif
     end do
  end do
  write(*,*) distances
end subroutine calculate_distances


recursive subroutine shortest_path(airport,stack,head,count,N,distances,shortestdistance,shortestroute)
  implicit none
  integer :: i,j,head,st=0,count,N
  integer :: stack(N)
  type(map_target) :: airport(N), shortestroute(N)
  real :: s,shortestdistance
  real :: distances(N,N)
  
  do i=2,N
   !Choose next point
  stack(head+1)=i

   !Check that point is not visited yet 
  do j=1,head
    if (stack(head+1)==stack(j)) st=1
  end do
  if (st==1) then 
	 st=0
  else
  
  !If point is visited jump over next statement
    !kutsutaan rekursiivista aliohjelmaa
    if (head < N-1) call shortest_path(airport,stack,head+1,count,N,distances,shortestdistance,shortestroute)
    !kun rekursio saavuttaa maksimisyvyyden (m=N-1) niin tulostetaan reitti, rekursion syvyys ja reitin pituus
    if (head == N-1) then
	  
	  count=count+1
	  !Calculate travelled distance
	  s=0
	  do j=1,N-1
         s=s+distances(stack(j),stack(j+1))
      enddo
      s=s+distances(stack(N),stack(1))
      
      if (s<shortestdistance) then
         shortestdistance=s
         write(*,*) s,stack
		 do j=1,N
	       shortestroute(j)=airport(stack(j)) 
	     enddo
	     
      end if
    end if
  endif
  enddo
end subroutine shortest_path

subroutine print_path(path)
  
  type(map_target) :: path(:)
  integer :: i

  do i=1,N
	write(*,*) shortestroute(i)%name,shortestroute(i)%lat*180.0/pi,shortestroute(i)%lon*180.0/pi
  enddo

end subroutine print_path

end program airportappro