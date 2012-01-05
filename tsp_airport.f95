program airportappro
  implicit none

  !Define new datatype for airports
  type map_target
    character(len = 20) :: name
    real :: lat,lon
  end type map_target
  
  !Define variables
  !
  ! N = number of cities 
  ! head, stack are help variables for shortest_path
  ! count = number of routes analyzed
  !
  integer :: N, count, st, head
  character(len = 20) :: filename
  type(map_target), allocatable :: airport(:), shortestroute(:)
  integer, allocatable :: stack(:)
  real, allocatable :: distances(:,:)
  real :: shortestdistance
  real, parameter :: deg_to_rad=3.141592/180.0
  
  write(*,*) "Type input file: "
  read(*,*) filename
  
  call read_file()
  write(*,*) "Found next airports from file:"
  call print_targets(airport)
  call calculate_distances()
  
  !Initialize variables for shortest_path
  stack(1:N)=1
  head=1
  count=0
  shortestdistance = 100000

  call shortest_path(stack,head)
  
  write(*,*)
  write(*,*) "Number of paths analyzed:", count
  write(*,*)
  write(*,*) "Shortest distance:", shortestdistance, "km"
  write(*,*) "Shortest path::"

  call print_targets(shortestroute)
  stop

contains


subroutine read_file()
  implicit none

  character(len = 20) :: d1 
  character(len = 1)  :: d5, d9
  real    :: d2,d3,d4,d6,d7,d8 
  integer :: ios, st, i
  
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
	
	!read airport names and coordinates and
	!transform coordinates to radians
	airport(i)%name = d1
	airport(i)%lat = (d2 + d3/60.0 + d4/3600.0)*deg_to_rad
	airport(i)%lon = (d6 + d7/60.0 + d8/3600.0)*deg_to_rad
	if(trim(d5)=='S') airport(i)%lat=-airport(i)%lat
	if(trim(d9)=='W') airport(i)%lon=-airport(i)%lon
  end do
  close(10)
end subroutine read_file 
 
 
subroutine calculate_distances()
  implicit none

  integer :: st, i, j
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
  !write(*,*) distances
end subroutine calculate_distances


recursive subroutine shortest_path(stack,head)
  implicit none
  integer :: i,j,head,st
  integer :: stack(N)
  real :: s

  st=0
  
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
    if (head < N-1) call shortest_path(stack,head+1)
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
         !write(*,'(F10.1, 11I3)') s,stack
		 do j=1,N
	       shortestroute(j)=airport(stack(j)) 
	     enddo
	     
      end if
    end if
  endif
  enddo
end subroutine shortest_path


subroutine print_targets(maptargets)
  implicit none

  type(map_target) :: maptargets(N),path(N)
  integer :: i
  character(len = 20) :: d1 
  character(len = 1)  :: d5, d9
  integer :: d2, d3, d4, d6, d7, d8

  path=maptargets
  path%lat=path%lat/deg_to_rad
  path%lon=path%lon/deg_to_rad

  do i=1,N
    d1=path(i)%name
  
    if(path(i)%lat < 0) then 
      d5='S'
      path(i)%lat = -path(i)%lat
    else
      d5='N'
    endif
    if(path(i)%lon < 0) then 
      d9='W'
      path(i)%lon = -path(i)%lon
    else
	  d9='E'
    endif
  
    d2 = Int(path(i)%lat)
    d3 = Int((path(i)%lat - d2) * 60.0)
    d4 = Int((path(i)%lat - d2 - d3/60.0) * 3600.0)  

    d6 = Int(path(i)%lon)
    d7 = Int((path(i)%lon - d6) * 60.0)
    d8 = Int((path(i)%lon - d6 - d7/60.0)*3600.0)

  
	!write(*,*) shortestroute(i)%name, shortestroute(i)%lat, shortestroute(i)%lon
    write(*,'(A20,I3,I3,I3,A2,I4,I3,I3,A2)') d1, d2, d3, d4, d5, d6, d7, d8, d9
     
  enddo

end subroutine print_targets

end program airportap