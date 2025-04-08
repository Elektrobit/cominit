def UID
def GID

properties([
    gitLabConnection('GitLab')
])

pipeline {
    agent {
      label 'agent'
    }
    environment {
        UID = sh(script: 'id -u', returnStdout: true).trim()
        GID = sh(script: 'id -g', returnStdout: true).trim()
    }
    options {
        buildDiscarder(logRotator(numToKeepStr: '4'))
        disableConcurrentBuilds()
        gitlabBuilds(builds: ["Build", "Analyse: Lint", "Analyse: Unit Tests", "Store result"])
    }
    stages {
        stage ('Setup') {
            agent {
                dockerfile {
                    dir 'ci'
                    reuseNode true
                    additionalBuildArgs "--build-arg USER=jenkins \
                        --build-arg UID=${env.UID} --build-arg GID=${env.GID}"
                    args "--privileged \
                        -v /etc/ssh/ssh_known_hosts:/etc/ssh/ssh_known_hosts:ro \
                        --tmpfs ${TMPDIR}:rw,size=787448k,mode=1777"
                }
            }
            stages {
                stage ('Build') {
                    steps {
                        gitlabCommitStatus("$STAGE_NAME") {
                            sh '''#!/bin/bash -xe
                              env
                              git clean -xdff
                              ci/build.sh
                            '''
                        }
                    }
                }
                stage ('Analyse: Lint') {
                    steps {
                        gitlabCommitStatus("$STAGE_NAME") {
                            sh '''#!/bin/bash -xe
                              ci/clang-tidy.sh
                              ci/lint-commits.sh origin/integration
                            '''
                        }
                    }
                }
                stage ('Analyse: Unit Tests') {
                    steps {
                        gitlabCommitStatus("$STAGE_NAME") {
                            sh '''#!/bin/bash -xe
                              ci/run-utests.sh
                            '''
                        }
                    }
                }
            }
        }
        stage('Store result') {
            steps {
                gitlabCommitStatus("$STAGE_NAME") {
                    archiveArtifacts 'result/**'
                }
            }
        }
    }
}

