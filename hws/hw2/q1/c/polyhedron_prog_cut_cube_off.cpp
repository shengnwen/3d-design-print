#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/polyhedron_cut_plane_3.h>
#include <iostream>
#include <fstream>
using namespace std;

template <class Poly>
typename Poly::Halfedge_handle make_cube_3( Poly& P) {
    // appends a cube of size [0,1]^3 to the polyhedron P.
    CGAL_precondition( P.is_valid());
    typedef typename Poly::Point_3         Point;
    typedef typename Poly::Plane_3         Plane;
    typedef typename Poly::Halfedge_handle Halfedge_handle;
    Halfedge_handle h = P.make_tetrahedron( Point( 1, 0, 0),
                                            Point( 0, 0, 1),
                                            Point( 0, 0, 0),
                                            Point( 0, 1, 0));
    Halfedge_handle g = h->next()->opposite()->next();
    P.split_edge( h->next());
    P.split_edge( g->next());
    P.split_edge( g);
    h->next()->vertex()->point()     = Point( 1, 0, 1);
    g->next()->vertex()->point()     = Point( 0, 1, 1);
    g->opposite()->vertex()->point() = Point( 1, 1, 0);
    Halfedge_handle f = P.split_facet( g->next(), g->next()->next()->next());
    Halfedge_handle e = P.split_edge( f);
    e->vertex()->point() = Point( 1, 1, 1);
    P.split_facet( e, f->next()->next());
    CGAL_postcondition( P.is_valid());
    g = h;
    g->facet()->plane() = Plane( g->vertex()->point(),
                                 g->next()->vertex()->point(),
                                 g->next()->next()->vertex()->point());
    g = h->opposite();
    g->facet()->plane() = Plane( g->vertex()->point(),
                                 g->next()->vertex()->point(),
                                 g->next()->next()->vertex()->point());
    g = h->next()->opposite();
    g->facet()->plane() = Plane( g->vertex()->point(),
                                 g->next()->vertex()->point(),
                                 g->next()->next()->vertex()->point());
    g = h->next()->next()->opposite();
    g->facet()->plane() = Plane( g->vertex()->point(),
                                 g->next()->vertex()->point(),
                                 g->next()->next()->vertex()->point());
    g = h->next()->next()->next()->opposite();
    g->facet()->plane() = Plane( g->vertex()->point(),
                                 g->next()->vertex()->point(),
                                 g->next()->next()->vertex()->point());
    g = g->next()->next()->opposite();
    g->facet()->plane() = Plane( g->vertex()->point(),
                                 g->next()->vertex()->point(),
                                 g->next()->next()->vertex()->point());
    return h;
}

typedef CGAL::Exact_predicates_inexact_constructions_kernel  Kernel;
typedef Kernel::Point_3                                      Point;
typedef Kernel::Plane_3                                      Plane;
typedef CGAL::Polyhedron_3<Kernel>                           Polyhedron;
typedef Polyhedron::Halfedge_handle                          Halfedge_handle;

bool  verbose   = false;
bool  binary    = false;
bool  noc       = false;
int main( int argc, char **argv) {
    if (argc != 2){
        cout << "Your should enter a file name.!" << endl;
        return 0;
    }
    Polyhedron P;
    Halfedge_handle h = make_cube_3( P);
    Plane pl = Plane( Point( 0.5, 0.0, 0.0),
                      Point( 0.0, 0.0, 1.5),
                      Point( 0.0, 0.5, 0.0));
    CGAL::polyhedron_cut_plane_3( P, h, pl);
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

    CGAL::Verbose_ostream vout( verbose);
    vout << argv[0] << ": verbosity on." << endl;

    const char* name    = "cout";
    ostream*  p_out = &cout;
    ofstream  out;
    string outFile = string(filename[0]);
    out.open(outFile);
    p_out = &out;

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
