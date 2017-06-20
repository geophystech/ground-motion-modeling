# ground-motion-modeling
The ground motion evaluation module (WORK IN PROGRESS)

## Libs
- GeographicLib v.1.48+
- Glib v.2.50+

## Dependencies
- GNU Make (tested on 4.2.1)
- GCC (testd on 5.4.0)

## Command-line interface
**Using:**
```
gmm <path_to_config_file> [path_to_s_file]
```
**Example with s-file**
```
bin/gmm examples/configs/gmm.conf examples/seisan/19-1248-22L.S201402
```
**Example without s-file:**
```
echo "51.92 143.04 13.0 6.0 0" | bin/gmm examples/configs/gmm.conf
```
The PGA xyz data `lon lat %g` will be saved to `gmm_pga_out.xyz` file in the same directory.

## AS2008 GMPE Model
`Abrahamson, Norman, and Walter Silva. "Summary of the Abrahamson & Silva NGA ground-motion relations." Earthquake spectra 24.1 (2008): 67-97.`

**The meaning of some variables in this implementation:**

`a12 * Frv` = 0, `a13 * Fnm` = 0, `a15 * Fas` = 0, `Fhw * f4(Rjb, Rrup, Rx, W, S, Ztor, Mw)` = 0, `f6(Ztor)` = 0, `f10(Z1.0, Vs30)` = 0.
