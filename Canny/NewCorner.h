
class Edge{
public:
    vector<Point> segment;
    int edgeid;
  
    		int start;			// Start index of edge in counter-clockwise sense in boundary
    		int end;			// End index of edge in counter-clockwise sense in boundary
    //		Edge(int i,int j);
    
    Edge(vector<Point> p, int i = 0 , int j = 0)
    {
        segment = p;
        edgeid = -1;
        start = (i);
        end = (j);
        
    }
};


class Polygon_corner {
public:
    double get_val(int ind1 , int ind2,Point point_bef, Point point_after,vector <Point> points);
    vector<Edge> corner_points(vector<Point> points , double epsilon, Mat image);
    vector<Point> RamerDouglasPeuckerAlgo( vector<Point> points , double epsilon );
    double shortestDistanceToSegment(Point C, Point A, Point B);
    double Dist( Point A, Point B);
    double Cross( Point A, Point B, Point C);
    double Dot( Point A, Point B, Point C);
};



/*class Edge{
 public:
 int i1 ,i2 ;
 Edge(int i,int j);
 };
 */


//	public static void main( String [] args ){
//
//		Scanner stdin = new Scanner(System.in);
//		int i;
//		int noOfPoints = stdin.nextInt();
//		vector<Point> points = new vector<Point>();
//		for( i = 0; i < noOfPoints; i++ ){
//			double tempX = stdin.nextDouble();
//			double tempY = stdin.nextDouble();
//                        points.add(new Point(tempX,tempY));
//		}
//		double epsilon = stdin.nextDouble();
//
//		vector<Point> reducedPoints = RamerDouglasPeuckerAlgo(points,epsilon);
//		System.out.println("No of Points : " + reducedPoints.size() );
//		for( i = 0; i < reducedPoints.size(); i++ ){
//			System.out.println(reducedPoints.get(i).x + " " + reducedPoints.get(i).y);
//		}
//	}

double Polygon_corner::Dist( Point A, Point B){
 	return sqrt( (A.x-B.x)*(A.x-B.x) + (A.y-B.y)*(A.y-B.y) );
}

double Polygon_corner::Dot( Point A, Point B, Point C){
 	double dot = ((B.x-A.x) * (C.x-B.x))  + ( (B.y-A.y) * (C.y-B.y) ) ;
 	return dot;
}

double Polygon_corner::Cross( Point A, Point B, Point C){
 	double cross = ( (B.x-A.x) * (C.y-A.y) )  - ( (B.y-A.y) * (C.x-A.x) ) ;
 	return cross;
}

double Polygon_corner::shortestDistanceToSegment(Point C, Point A, Point B) {
    
 	double dist = Cross(A,B,C)/Dist(A,B);
 	double dot1 = Dot(A,B,C);
 	if( dot1 > 0 ) return Dist(B,C);
 	double dot2 = Dot(B,A,C);
 	if( dot2 > 0 ) return Dist(A,C);
 	return abs(dist);
}

vector<Point> Polygon_corner::RamerDouglasPeuckerAlgo( vector<Point> points , double epsilon ){
	vector<Point> resultPoints;
	double dmax = 0;
	int storeIndex = -1;
    
	for( int i = 1; i < (int)points.size()-1; i++ ){
		double d = shortestDistanceToSegment(points.at(i), points.at(0), points.at(points.size()-1) );
		if( d > dmax ){
			//cout << "Hurray";
			dmax = d;
			storeIndex = i;
		}
	}
    
	if( dmax > epsilon ){
		//cout << "Hurray";
		vector<Point> temp_3(points.begin(), points.begin()+storeIndex);
		vector<Point> temp_4(points.begin()+storeIndex, points.begin()+ points.size()-1);
        
		vector<Point> rec1Result = RamerDouglasPeuckerAlgo( temp_3 , epsilon);
		vector<Point> rec2Result = RamerDouglasPeuckerAlgo( temp_4, epsilon);
        
		resultPoints.insert(resultPoints.begin(), rec1Result.begin(), rec1Result.end());
		resultPoints.pop_back();
		resultPoints.insert(resultPoints.end(), rec2Result.begin(), rec2Result.end());
		//cout << resultPoints.size() << endl;
	}
	else {
		resultPoints.push_back(points.at(0));
		resultPoints.push_back(points.at(points.size()-1));
	}
    
	return resultPoints;
}


vector<Edge> Polygon_corner::corner_points(vector<Point> points , double epsilon, Mat image){
    
	vector<Point> reducedPoints = RamerDouglasPeuckerAlgo(points,epsilon);
	int xcoord, ycoord, i, j;
	for(i=0; i < (int)points.size(); i++){
		Point p = points[i];
		//cout << "X = " << p.x << " Y = " << p.y << endl;
		xcoord = (int) p.x ; ycoord = (int) p.y ;
		image.at<Vec3b>(ycoord, xcoord)[0] = 255;
		image.at<Vec3b>(ycoord, xcoord)[1] = 0;
		image.at<Vec3b>(ycoord, xcoord)[2] = 0;
	}
    
	int boxsize = 30;
	for(int  k=0; k < (int)reducedPoints.size(); k++){
		Point p = reducedPoints.at(k);
		cout << "X = " << p.x <<  " Y = " <<  p.y << endl;
		xcoord = (int) p.x ; ycoord = (int) p.y ;
		for(i = xcoord ; i <= xcoord + boxsize ; i++) //-> Y direction
			for(j = ycoord; j <= ycoord + boxsize ; j++)// -> X direction
			{
				image.at<Vec3b>(j, i)[0] = 255;
				image.at<Vec3b>(j, i)[1] = 0;
				image.at<Vec3b>(j, i)[2] = 0;
			}
	}
//	imwrite( "./src/"+f_name+".jpg", image );
    
	vector<Edge> Edges;
	vector<Point> p;
	int rpindex = 0;					// index of Reduced point vector
    
	for(int i = 0; i < (int)points.size(); i++)
	{
		if(points[i].x == reducedPoints[rpindex].x && points[i].y == reducedPoints[rpindex].y)
		{
			if(!p.empty())
			{
				p.push_back(points[i]);
				Edge edge(p);
				Edges.push_back(edge);
				p.clear();
				p.push_back(points[i]);
			}
			else
				p.push_back(points[i]);
			rpindex++;
		}
		else
			p.push_back(points[i]);
	}
	if(!p.empty())
	{
		if(!points.empty())
			p.push_back(points[0]);
		Edge edge(p);
		Edges.push_back(edge);
	}
//	FILE *fp;
//	fp = fopen("/Users/rohanraja/Downloads/out_edge.txt","w");
//	for(int i = 0; i < (int)Edges.size(); i++)
//	{
//		for(int j = 0; j < (int)Edges[i].segment.size(); j++)
//		{
//			fprintf(fp, "%d %d\n", Edges[i].segment[j].x, Edges[i].segment[j].y);
//		}
//		fprintf(fp, "\n\n");
//	}
//	fclose(fp);
	return Edges;
}




class feature{
public:
    double angle;
    double distance;
    feature(double a, double d);
};

class Edge_Envelope{
public:
	vector<feature> forward, backward;
	Point fStart,fEnd ;
	Edge_Envelope(vector<feature> f , vector<feature> b, Point S , Point E );
	Edge_Envelope();
	double deviation(double);
};

class Feature_vector{
public:
    
	vector<feature> calculate(vector <Point> list);
	Edge_Envelope fb_envelope(vector<Point> list);
	vector<Point> reverse(vector<Point> l);
    
};


feature::feature(double a, double d){
	angle = a;
	distance = d;
}


/*class Edge_Envelope{
 //	ArrayList<feature> edges = new ArrayList<feature>();
 vector<feature> edges;
 Point Start,End ;
 Edge_Envelope(vector<feature> edge , Point S , Point E );
 };*/

Edge_Envelope::Edge_Envelope(vector<feature> f ,vector<feature> b, Point S , Point E ){
	forward = f; backward = b ; fStart = S ; fEnd = E;
}

double Edge_Envelope::deviation(double dparam)
{
	double sum = 0,count = 0;
	for(int i = 0; i < (int)forward.size(); i++)
	{
		feature e = forward[i];
		if(forward[i].distance > dparam)
			count++;
		sum += (e.distance * e.distance * e.distance);
	}
    
	float stlength = norm(fStart - fEnd);
    
	sum = count/forward.size();
    
    if(stlength < 60)
        sum = 0;
	return sum;
}
/*class Feature_vector{
 public:
 Edge_Envelope calculate(vector<Point> list );
 };
 */
vector<Point> Feature_vector::reverse(vector<Point> l){
	vector<Point> rev;
	//cout << l.size() << endl;
	for(int i = (int)l.size()-1; i >= 0; i--){
		rev.push_back(l[i]);
	}
	return rev;
}

Edge_Envelope Feature_vector::fb_envelope(vector<Point> list){
    
	vector<feature> f = calculate(list);
	vector<feature> b = calculate(reverse(list));
	Edge_Envelope envelope(f,b,list[0],list[list.size()-1]);
    //	cout << "Start " << list[0] << " End " << list[list.size()-1] << " Deviation " << envelope.deviation() << endl;
	return envelope;
}

vector<feature> Feature_vector::calculate(vector <Point> list ){
	double m_initial,ang_initial,m_temp,ang_temp;
	int size = list.size();
	double d;
	vector<feature> Cedges;
    
	if( list[0].x != list[size - 1].x ){
		m_initial = (list[0].y - list[size - 1].y)/(double)(list[0].x - list[size - 1].x);
		ang_initial = atan(m_initial);
		if(ang_initial < 0) ang_initial += M_PI;
	}
	else ang_initial = M_PI/2 ;
    
	for(int i = 1; i< (int)list.size() - 1 ; i++){
		if( list.at(0).x !=  list.at(i).x){
			m_temp = (list.at(0).y - list.at(i).y)/(double)(list.at(0).x - list.at(i).x);
			ang_temp = atan(m_temp);
			if(ang_temp < 0) ang_temp += M_PI;
		}
		else
			ang_temp =  M_PI/2 ;
		d = sqrt(pow((list.at(i).y - list.at(0).y),2) + pow((list.at(i).x - list.at(0).x),2) );
		d = abs(d * sin(ang_initial - ang_temp));
		feature tedge (ang_initial - ang_temp, d);
		Cedges.push_back(tedge);
	}
	return Cedges;
}

// Input contour + Name of contour file
// Add the files for Polygon_corner.cpp , Feature_vector.cpp

struct edpair
{
    vector<pair<Point,Point> > vp;
    vector<double> dev ;
    vector<Point> startIDXs;
};

edpair getnonlinearedgeendpt(vector<Point> c,Mat image){
	edpair nonlinearedges;
    Polygon_corner pc;
	Feature_vector fv;
	vector<Edge> Edges;
    
	
//
//    
//
//	// change the two parameters as per ur requirement
	double epsilon = 150 ;
    double dparam = 2;
	double max_div = 0.55; //( < 1)
	Edges = pc.corner_points(c, epsilon, image);
    
    
	for(int j = 0; j < (int)Edges.size(); j++){
//		//cout << " 1: " << index1 << " 2: " << index2 <<  "11::" << c.boundary[index1] << "2::" << c.boundary[index2] << endl;
		Edge_Envelope envelope = fv.fb_envelope(Edges[j].segment);
		double deviation = envelope.deviation(dparam);
		cout << "deviation : " << deviation << endl;
		if(deviation > max_div || 1) {
			nonlinearedges.vp.push_back(make_pair(Edges[j].segment.at(0),Edges[j].segment.at(Edges[j].segment.size() - 1)));
            nonlinearedges.dev.push_back(deviation); // = deviation;
		}
	}
	return nonlinearedges;
}
