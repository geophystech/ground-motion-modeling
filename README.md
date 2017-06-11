# ground-motion-modeling
The ground motion evaluation module

# Libs

- GeographicLib v.1.48
- Glib v.2.50

# Dependencies

# Desktop

**Using:**
```
gmm <path_to_config_file> [path_to_s_file]
```

**Example:**
```
bin/gmm examples/configs/gmm.conf examples/seisan/19-1248-22L.S201402
```

The PGA xyz data `lon lat %g` will be saved to `gmm_pga_out.xyz` file in the same directory.

**Example without s-file:**
```
echo "51.92 143.04 13.0 6.0 0" | bin/gmm examples/configs/gmm.conf
```
Out:
```
READING config file 'examples/configs/gmm.conf'

--------------AS2008 PARAMETERS-----------------
 a1 = 0.804000          a2 = -0.967900
 a3 = 0.265000          a4 = -0.231000
 a5 = -0.398000         a8 = -0.037200
 a10 = 0.944500         a18 = -0.006700
 b = -1.186000          c = 1.880000
 c1 = 6.750000          c4 = 4.500000
 n = 1.180000           vlin = 865.100000
 v1 = 1500.00           vs30_1100 = 1100.00

No S-file given, please enter earthquake parameters
Lat     Lon     Depth   Ml      Mw:

--------------EARTHQUAKE LOCATION---------------
 lat = 51.920000        lon = 143.040000
 depth = 13.000000      Ml = 6.000000
 Mw = 0.000000

Time to read vs30 file takes 0.000000 seconds

Total lines in grid file = 17

Time to AS2008 modeling file takes 0.000000 seconds

 PGA grid printed to gmm_pga_out.xyz file
```
