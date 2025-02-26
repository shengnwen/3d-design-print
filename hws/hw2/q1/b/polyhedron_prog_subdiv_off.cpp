#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <string>

using namespace std;

typedef CGAL::Simple_cartesian<double>                       Kernel;
typedef Kernel::Vector_3                                     Vector;
typedef Kernel::Point_3                                      Point;
typedef CGAL::Polyhedron_3<Kernel>                           Polyhedron;

typedef Polyhedron::Vertex                                   Vertex;
typedef Polyhedron::Vertex_iterator                          Vertex_iterator;
typedef Polyhedron::Halfedge_handle                          Halfedge_handle;
typedef Polyhedron::Edge_iterator                            Edge_iterator;
typedef Polyhedron::Facet_iterator                           Facet_iterator;
typedef Polyhedron::Halfedge_around_vertex_const_circulator  HV_circulator;
typedef Polyhedron::Halfedge_around_facet_circulator         HF_circulator;

void create_center_vertex( Polyhedron& P, Facet_iterator f) {
    Vector vec( 0.0, 0.0, 0.0);
    std::size_t order = 0;
    HF_circulator h = f->facet_begin();
    do {
        vec = vec + ( h->vertex()->point() - CGAL::ORIGIN);
        ++ order;
    } while ( ++h != f->facet_begin());
    CGAL_assertion( order >= 3); // guaranteed by definition of polyhedron
    Point center =  CGAL::ORIGIN + (vec / static_cast<double>(order));
    Halfedge_handle new_center = P.create_center_vertex( f->halfedge());
    new_center->vertex()->point() = center;
}

struct Smooth_old_vertex {
    Point operator()( const Vertex& v) const {
        CGAL_precondition((CGAL::circulator_size( v.vertex_begin()) & 1) == 0);
        std::size_t degree = CGAL::circulator_size( v.vertex_begin()) / 2;
        double alpha = ( 4.0 - 2.0 * std::cos( 2.0 * CGAL_PI / degree)) / 9.0;
        Vector vec = (v.point() - CGAL::ORIGIN) * ( 1.0 - alpha);
        HV_circulator h = v.vertex_begin();
        do {
            vec = vec + ( h->opposite()->vertex()->point() - CGAL::ORIGIN)
              * alpha / static_cast<double>(degree);
            ++ h;
            CGAL_assertion( h != v.vertex_begin()); // even degree guaranteed
            ++ h;
        } while ( h != v.vertex_begin());
        return (CGAL::ORIGIN + vec);
    }
};

void flip_edge( Polyhedron& P, Halfedge_handle e) {
    Halfedge_handle h = e->next();
    P.join_facet( e);
    P.split_facet( h, h->next()->next());
}

void subdiv( Polyhedron& P) {
    if ( P.size_of_facets() == 0)
        return;
    // We use that new vertices/halfedges/facets are appended at the end.
    std::size_t nv = P.size_of_vertices();
    Vertex_iterator last_v = P.vertices_end();
    -- last_v;  // the last of the old vertices
    Edge_iterator last_e = P.edges_end();
    -- last_e;  // the last of the old edges
    Facet_iterator last_f = P.facets_end();
    -- last_f;  // the last of the old facets

    Facet_iterator f = P.facets_begin();    // create new center vertices
    do {
        create_center_vertex( P, f);
    } while ( f++ != last_f);

    std::vector<Point> pts;                    // smooth the old vertices
    pts.reserve( nv);  // get intermediate space for the new points
    ++ last_v; // make it the past-the-end position again
    std::transform( P.vertices_begin(), last_v, std::back_inserter( pts),
                    Smooth_old_vertex());
    std::copy( pts.begin(), pts.end(), P.points_begin());

    Edge_iterator e = P.edges_begin();              // flip the old edges
    ++ last_e; // make it the past-the-end position again
    while ( e != last_e) {
        Halfedge_handle h = e;
        ++e; // careful, incr. before flip since flip destroys current edge
        flip_edge( P, h);
    };
    CGAL_postcondition( P.is_valid());
}



bool  verbose   = false;
bool  binary    = false;
bool  noc       = false;

int main( int argc, char **argv) {
    int n = 0; // number of filenames
    char *filename[2];
    bool help = false;
    for (int i = 1; i < argc; i++) { // check commandline options
        if ( strcmp( "-v", argv[i]) == 0)
            verbose = true;
        else if ( strcmp( "-b", argv[i]) == 0)
            binary = true;
        else if ( strcmp( "-noc", argv[i]) == 0)
            noc = true;
        else if ( (strcmp( "-h", argv[i]) == 0) ||
                  (strcmp( "-help", argv[i]) == 0))
            help = true;
        else if ( n < 2 ) {
            filename[ n++] = argv[i];
        } else {
            ++n;
            break;
        }
    }
    if ((n > 2) || help) {
        if ( ! help)
            cerr << "Error: in parameter list" << endl;
        cerr << "Usage: " << argv[0] << " [<options>] [<infile> [<outfile>]]"
        << endl;
        cerr << "       copy a polyhedron in OFF." << endl;
        cerr << "       -b      binary." << endl;
        cerr << "       -noc    no comments in file." << endl;
        cerr << "       -v      verbose." << endl;
        exit( ! help);
    }

    CGAL::Verbose_ostream vout( verbose);
    vout << argv[0] << ": verbosity on." << endl;

    const char*  name = "cin";
    istream*     p_in = &cin;
    ifstream     in;
    if ( n > 0) {
        in.open( filename[0]);
        p_in = &in;
        name = filename[0];
    }
    if ( !*p_in) {
        cerr << argv[0] << ": error: cannot open file '"<< name
        << "' for reading." <<endl;
        exit( 1);
    }

    vout << "ifstream(" << name << ") >> CGAL::Polyhedron_3 ..." << endl;
    Polyhedron P;
    (*p_in) >> P;
    vout << "    .... done." << endl;


    P.normalize_border();
    if ( P.size_of_border_edges() != 0) {
        std::cerr << "The input object has border edges. Cannot subdivide."
                  << std::endl;
        std::exit(1);
    }
    subdiv( P);

    if ( !*p_in) {
        cerr << argv[0] << " read error: while reading file '"<< name << "'."
        << endl;
        exit( 1);
    }


    name    = "cout";
    ostream*  p_out = &cout;
    ofstream  out;
//    if ( n > 1) {
    string inFile = string(filename[0]);
    string outFile = inFile.insert(inFile.length() - 4, "_sd");
//    cout << outFile;
    out.open(outFile);
    p_out = &out;
    name = filename[1];
//    }
    if ( !*p_out) {
        cerr << argv[0] << ": error: cannot open file '"<< name
        << "' for writing." <<endl;
        exit( 1);
    }

    if ( binary) {
        vout << "CGAL::set_binary_mode( ofstream(" << name << "))" << endl;
        CGAL::set_binary_mode( *p_out);
    } else if ( noc) {
        vout << "CGAL::set_ascii_mode( ofstream(" << name << "))" << endl;
        CGAL::set_ascii_mode( *p_out);
    } else {
        vout << "CGAL::set_pretty_mode( ofstream(" << name << "))" << endl;
        CGAL::set_pretty_mode( *p_out);
    }

    vout << "ofstream(" << name << ") << CGAL::Polyhedron_3 ..." << endl;
    (*p_out) << P;
    vout << "    .... done." << endl;

    if ( !*p_out) {
        cerr << argv[0] << " write error: while writing file '"<< name << "'."
        << endl;
        exit( 1);
    }
    return 0;
}

//int main() {
//    Polyhedron P;
//    std::cin >> P;
//    P.normalize_border();
//    if ( P.size_of_border_edges() != 0) {
//        std::cerr << "The input object has border edges. Cannot subdivide."
//                  << std::endl;
//        std::exit(1);
//    }
//    subdiv( P);
//    std::cout << P;
//    return 0;
//}
