# How to obtain general trigger information

The EDAnalyzer in this example contains several C++ snippets on how to access trigger information such as metadata, prescales, module information, etc.
Many of these snippets are commented out in the code, which can be found in the `src` directory.  Examples on how to access specific information can be found in other packages in ths repository.
 
## Usage Instruction

First you have to create a [VM](http://opendata.cern.ch/docs/cms-virtual-machine-2011 "CMS 2011 Virtual Machines: How to install") from the CMS Open Data website or set up a [Docker container](http://opendata.cern.ch/docs/cms-guide-docker). 

Then follow these steps:

- Create a CMSSW environment (if using the Docker container, this step can be skipped): 

    ```
    cmsrel CMSSW_5_3_32
    ```

- Change to the CMSSW_5_3_32/src/ directory:

    ```
    cd CMSSW_5_3_32/src/
    ```

- Initialize the CMSSW environment:

  ```
  cmsenv
  ```

- Obtain the code from git and move it to the `src` area:

  ```
  git clone git://github.com/cms-legacydata-analyses/TriggerInfoTool.git
  cd TriggerInfoTool
  ```
  

- Go to the TriggerInfoTool/GeneralInfoAnalyzer area.  Note that the code lives under `src`

  ```
  cd GeneralInfoAnalyzer
  ```

- Compile everything:

  ```
  scram b
  ```

- Make a soft link to the python configuration file

```
ln -s python/triggerinfoanalyzer_cfg.py .
```

- Make symbolic links to the conditions database

```
ln -sf /cvmfs/cms-opendata-conddb.cern.ch/FT_53_LV5_AN1_RUNA FT_53_LV5_AN1
ln -sf /cvmfs/cms-opendata-conddb.cern.ch/FT_53_LV5_AN1_RUNA.db FT_53_LV5_AN1_RUNA.db
```

- Make sure the `cms-opendata-conddb.cern.ch` directory has actually expanded in your VM or Docker container.  One way of doing this is executing:

```
ls -l
ls -l /cvmfs/
```

You should now see the `cms-opendata-conddb.cern.ch` link in the `/cvmfs` area.


- Run the CMSSW executable in the background and dump the output in a log file

```
cmsRun triggerinfoanalyzer_cfg.py > full.log 2>&1 &
```

- Check the development of the job if needed:

```
tail -f full.log
```

*NOTE*: The first time you execute the job, it will take a long time (depending on your connection speed) to the point that it looks like it is not doing anything.  That is fine.  This is because the database payload files will be downloaded/cached locally in the VM or container.  Later attempts should be faster, however.
