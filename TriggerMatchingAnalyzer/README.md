# How to match a trigger object to a reconstructed track 

Sometimes, it is necessary to match the reconstructed object (particle) to the object (particle) that actually fired the trigger.  The triggger-level information about these objects can be retrieved using the [TriggerEvent CMSSW object](https://github.com/cms-sw/cmssw/blob/CMSSW_5_3_X/DataFormats/HLTReco/interface/TriggerEvent.h).  In this package we show an example on how to match reconstructed tracks to objects that fired a trigger (or possible set of triggers) that contain a specific module, hltSingleJet190Regional, for this demonstration (we specify this through configuration).

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
  

- Go to the TriggerInfoTool/ModuleInTriggerAnalyzer area.  Note that the code lives under `src`

  ```
  cd ModuleInTriggerAnalyzer
  ```

- Compile everything:

  ```
  scram b
  ```

- Make a soft link to the python configuration file

```
ln -s python/moduleinfoanalyzer_cfg.py.
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
cmsRun moduleinfoanalyzer_cfg.py > full.log 2>&1 &
```

- Check the development of the job if needed:

```
tail -f full.log
```

*NOTE*: The first time you execute the job, it will take a long time (depending on your connection speed) to the point that it looks like it is not doing anything.  That is fine.  This is because the database payload files will be downloaded/cached locally in the VM or container.  Later attempts should be faster, however.
