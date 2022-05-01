from cpython.mem cimport PyMem_Malloc, PyMem_Free

cdef extern from "steam.h":
    ctypedef struct SteamState_R1:
        double p, T
    ctypedef struct SteamState_R2:
        double p, T
    ctypedef struct SteamState_R3:
        double rho, T
    ctypedef struct SteamState_R4:
        double T, x

#ctypedef union steam_R:
    


cdef extern from "steam.h":
    ctypedef struct SteamState:
        char region
        SteamState_R1 R1
        SteamState_R2 R2
        SteamState_R3 R3
        SteamState_R4 R4
    double freesteam_p(SteamState S)
    double freesteam_T(SteamState S)
    double freesteam_rho(SteamState S)
    double freesteam_v(SteamState S)
    double freesteam_u(SteamState S)
    double freesteam_h(SteamState S)
    double freesteam_s(SteamState S)
    double freesteam_cp(SteamState S)
    double freesteam_cv(SteamState S)
    double freesteam_w(SteamState S)
    double freesteam_x(SteamState S)
    double freesteam_mu(SteamState S)
    double freesteam_k(SteamState S)

cdef extern from 'steam_ps.h':
    SteamState freesteam_set_ps(double p, double s)

cdef extern from 'steam_ph.h':
    SteamState freesteam_set_ph(double p, double h)

cdef extern from 'steam_pT.h':
    SteamState freesteam_set_pT(double p, double T)

cdef extern from 'steam_pu.h':
    SteamState freesteam_set_pu(double p, double u)

cdef extern from 'steam_pv.h':
    SteamState freesteam_set_pv(double p, double v)

cdef extern from 'steam_Ts.h':
    SteamState freesteam_set_Ts(double T, double s)

cdef extern from 'steam_Tx.h':
    SteamState freesteam_set_Tx(double T, double x)

cdef extern from 'steam_uv.h':
    SteamState freesteam_set_uv(double u, double v)




cdef class Steam:
    cdef SteamState state

    cpdef int get_region(self):
        return int(self.state.region)

    cpdef double get_p(self):
        return freesteam_p(self.state)
    
    cpdef double get_T(self):
        return freesteam_T(self.state)
    
    cpdef double get_rho(self):
        return freesteam_rho(self.state)
    
    cpdef double get_v(self):
        return freesteam_v(self.state)
    
    cpdef double get_u(self):
        return freesteam_u(self.state)
    
    cpdef double get_h(self):
        return freesteam_h(self.state)
    
    cpdef double get_s(self):
        return freesteam_s(self.state)
    
    cpdef double get_cp(self):
        return freesteam_cp(self.state)
    
    cpdef double get_cv(self):
        return freesteam_cv(self.state)
        
    cpdef double get_w(self):
        return freesteam_w(self.state)
    
    cpdef double get_x(self):
        return freesteam_x(self.state)
    
    cpdef double get_mu(self):
        return freesteam_mu(self.state)
    
    cpdef double get_k(self):
        return freesteam_k(self.state)


cpdef Steam set_ps(double p, double s):
    cdef Steam st
    st=Steam()
    st.state = freesteam_set_ps(p, s)
    return st

cpdef Steam set_ph(double p, double h):
    cdef Steam st
    st=Steam()
    st.state = freesteam_set_ph(p, h)
    return st

cpdef Steam set_pu(double p, double u):
    cdef Steam st
    st=Steam()
    st.state = freesteam_set_pu(p, u)
    return st

cpdef Steam set_pv(double p, double v):
    cdef Steam st
    st=Steam()
    st.state = freesteam_set_pv(p, v)
    return st

cpdef Steam set_Ts(double T, double s):
    cdef Steam st
    st=Steam()
    st.state = freesteam_set_Ts(T, s)
    return st

cpdef Steam set_Tx(double T, double x):
    cdef Steam st
    st=Steam()
    st.state = freesteam_set_Tx(T, x)
    return st

cpdef Steam set_uv(double u, double v):
    cdef Steam st
    st=Steam()
    st.state = freesteam_set_uv(u, v)
    return st




cdef extern from 'region4.h':
    double freesteam_region4_psat_T(double T)
    double freesteam_region4_Tsat_p(double p)
    double freesteam_region4_rhof_T(double T)
    double freesteam_region4_rhog_T(double T)
    double freesteam_region4_dpsatdT_T(double T)


cpdef double psat_T(double T):
    return freesteam_region4_psat_T(T)

cpdef double Tsat_p(double p):
    return freesteam_region4_Tsat_p(p)
    
cpdef double rhof_T(double T):
    return freesteam_region4_rhof_T(T)

cpdef double rhog_T(double T):
    return freesteam_region4_rhog_T(T)

cpdef double dpsatdt_T(double T):
    return freesteam_region4_dpsatdT_T(T)

